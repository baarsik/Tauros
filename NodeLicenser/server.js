var express = require('express');
var fs = require('fs');
var mysql = require('mysql');

var app = express();
var dbconfig = {
    host: "node.cerberus.by",
    user: "node_user",
    password: "SXNLLV8tlX",
    database: "node_db"
};

// f - fileId
// h - md5 hash
app.get('/check', function(req, res) {
    if (!req.query.f || !req.query.h) {
        res.sendStatus(400);
        return;
    }

    var conn = mysql.createConnection(dbconfig);
    conn.connect();
    conn.query(
        "SELECT * FROM `files` WHERE `fileId` = ? ORDER BY `date` DESC", [req.query.f],
        function (qErr, rows) {
            conn.end();
            if (qErr) {
                res.sendStatus(500);
                console.log(qErr);
                return;
            }

            if (rows.length === 0) {
                res.sendStatus(404);
                return;
            }

            var md5 = rows[0].md5.toUpperCase();
            var currentmd5 = req.query.h.toUpperCase();
            if (!md5) {
                res.sendStatus(500);
                return;
            }
            var isUpToDate = currentmd5 === md5;
            var latestVersion = rows[0].version;
            if (isUpToDate) {
                var currentVersion = latestVersion;
                res.status(200).send({
                    "result": true,
                    "currentVersion": currentVersion,
                    "latestVersion": latestVersion
                });
            } else {
                conn = mysql.createConnection(dbconfig);
                conn.connect();
                conn.query(
                    "SELECT * FROM `files`" +
                    "WHERE (`fileId` = ? AND `md5` = ?)" +
                    "ORDER BY `date` DESC",
                    [req.query.f, currentmd5],
                    function(qErr1, rows1) {
                        conn.end();
                        var currentVersion;
                        if (qErr1 || rows1.length === 0)
                            currentVersion = "Unknown";
                        else
                            currentVersion = rows1[0].version;

                        res.status(200).send({
                            "result": false,
                            "currentVersion": currentVersion,
                            "latestVersion": latestVersion
                        });
                    }
                );
            }
        }
    );
});

// f - fileId
app.get('/dl', function (req, res) {
    if (!req.query.f) {
        res.sendStatus(400);
        return;
    }

    var conn = mysql.createConnection(dbconfig);
    conn.connect();
    conn.query(
        "SELECT * FROM `files` WHERE `fileId` = ?", [req.query.f],
        function (qErr, rows) {
            conn.end();
            if (qErr) {
                res.sendStatus(500);
                console.log(qErr);
                return;
            }
            
            if (rows.length === 0) {
                res.sendStatus(404);
                return;
            }
            
            var fileName = rows[0].fileName;
            var fileUrl = rows[0].fileUrl;
            
            if (!fileName || !fileUrl) {
                res.sendStatus(404);
                return;
            }
            
            var path = __dirname + '/uploads/' + req.query.f + '/' + fileUrl;
            if (!fs.existsSync(path)) {
                res.sendStatus(404);
                return;
            };
            var file = fs.readFileSync(path);
            res.setHeader('Content-disposition', 'attachment; filename=' + fileName);
            res.setHeader('Content-Length', file.length);
            res.write(file, 'binary');
            res.end();
        }
    );
});

app.listen(32523);