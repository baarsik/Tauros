#pragma once

#include <Windows.h>
#include <Psapi.h>
#include "SourceEngine/SDK.hpp"
#include "CSGOStructs.hpp"

extern HMODULE g_hLib;
extern HWND g_hWnd;

inline float DEG2RAD(float deg)
{
	return deg * 3.14159265358979323846f / 180.0f;
}

struct FontSize
{
	int width;
	int height;
};

class Utils
{
public:
	static std::string GetDllDir()
	{
		auto path = GetDllDirW();
		return std::string(path.begin(), path.end());
	}

	static std::wstring GetDllDirW()
	{
		wchar_t path[1024];
		GetModuleFileNameW(g_hLib, path, 1024);
		std::wstring str(path);
		auto pos = str.rfind('\\');
		if (pos == std::string::npos)
			return nullptr;

		str.erase(pos + 1, str.length() - pos - 1);
		return str;
	}

	static std::string GetGameDir()
	{
		auto path = GetGameDirW();
		return std::string(path.begin(), path.end());
	}

	static std::wstring GetGameDirW()
	{
		wchar_t path[1024];
		GetModuleFileNameW(nullptr, path, 1024);
		std::wstring str(path);
		auto pos = str.rfind('\\');
		if (pos == std::string::npos)
			return nullptr;

		str.erase(pos + 1, str.length() - pos - 1);
		return str;
	}

	static void FlashTaskbarIcon()
	{
		FLASHWINFO fi;
		fi.cbSize = sizeof(FLASHWINFO);
		fi.hwnd = g_hWnd;
		fi.dwFlags = FLASHW_ALL | FLASHW_TIMERNOFG;
		fi.uCount = 0;
		fi.dwTimeout = 0;
		FlashWindowEx(&fi);
	}

    static se::Vector GetEntityBone(se::IClientEntity* pEntity, se::ECSPlayerBones Bone)
    {
        se::matrix3x4_t boneMatrix[128];

        if(!pEntity->SetupBones(boneMatrix, 128, 0x100, se::Interfaces::Engine()->GetLastTimeStamp()))
            return se::Vector();

	    auto hitbox = boneMatrix[Bone];

        return se::Vector(hitbox[0][3], hitbox[1][3], hitbox[2][3]);
    }

    static bool ScreenTransform(const se::Vector& point, se::Vector& screen)
    {
        const auto& w2sMatrix = se::Interfaces::Engine()->WorldToScreenMatrix();
        screen.x = w2sMatrix.m[0][0] * point.x + w2sMatrix.m[0][1] * point.y + w2sMatrix.m[0][2] * point.z + w2sMatrix.m[0][3];
        screen.y = w2sMatrix.m[1][0] * point.x + w2sMatrix.m[1][1] * point.y + w2sMatrix.m[1][2] * point.z + w2sMatrix.m[1][3];
        screen.z = 0.0f;

	    auto w = w2sMatrix.m[3][0] * point.x + w2sMatrix.m[3][1] * point.y + w2sMatrix.m[3][2] * point.z + w2sMatrix.m[3][3];

        if(w < 0.001f) {
            screen.x *= 100000;
            screen.y *= 100000;
            return true;
        }

	    auto invw = 1.0f / w;
        screen.x *= invw;
        screen.y *= invw;
        return false;
    }

    static bool WorldToScreen(const se::Vector &origin, se::Vector &screen)
    {
        if(!ScreenTransform(origin, screen)) {
            int iScreenWidth, iScreenHeight;
            se::Interfaces::Engine()->GetScreenSize(iScreenWidth, iScreenHeight);

            screen.x = (iScreenWidth / 2.0f) + (screen.x * iScreenWidth) / 2;
            screen.y = (iScreenHeight / 2.0f) - (screen.y * iScreenHeight) / 2;

            return true;
        }
        return false;
    }

    static uint64_t FindSignature(const char* szModule, const char* szSignature)
    {
        //CREDITS: learn_more
    #define INRANGE(x,a,b)  (x >= a && x <= b) 
    #define getBits( x )    (INRANGE((x&(~0x20)),'A','F') ? ((x&(~0x20)) - 'A' + 0xa) : (INRANGE(x,'0','9') ? x - '0' : 0))
    #define getByte( x )    (getBits(x[0]) << 4 | getBits(x[1]))

        MODULEINFO modInfo;
        GetModuleInformation(GetCurrentProcess(), GetModuleHandleA(szModule), &modInfo, sizeof(MODULEINFO));
	    auto startAddress = reinterpret_cast<DWORD>(modInfo.lpBaseOfDll);
	    auto endAddress = startAddress + modInfo.SizeOfImage;
	    auto pat = szSignature;
        DWORD firstMatch = 0;
        for(auto pCur = startAddress; pCur < endAddress; pCur++) {
            if(!*pat) return firstMatch;
            if(*(PBYTE)pat == '\?' || *reinterpret_cast<BYTE*>(pCur) == getByte(pat)) {
                if(!firstMatch) firstMatch = pCur;
                if(!pat[2]) return firstMatch;
                if(*(PWORD)pat == '\?\?' || *(PBYTE)pat != '\?') pat += 3;
                else pat += 2;    //one ?
            } else {
                pat = szSignature;
                firstMatch = 0;
            }
        }
        return NULL;
    }

    static bool Clamp(se::QAngle &angles)
    {
	    auto a = angles;
        Normalize(a);
        ClampAngles(a);

        if(isnan(a.x) || isinf(a.x) || isnan(a.y) || isinf(a.y) || isnan(a.z) || isinf(a.z))
            return false;
		
    	angles = a;
        return true;
    }

	static void AngleVectors(const se::Vector &angles, se::Vector *forward)
	{
		//SinCos(DEG2RAD(angles[1]), &sy, &cy);
		auto sy = sin(DEG2RAD(angles[1]));
		auto cy = cos(DEG2RAD(angles[1]));
		//SinCos(DEG2RAD(angles[0]), &sp, &cp);
		auto sp = sin(DEG2RAD(angles[0]));
		auto cp = cos(DEG2RAD(angles[0]));

		forward->x = cp*cy;
		forward->y = cp*sy;
		forward->z = -sp;
	}

	static se::QAngle CalcAngle(se::Vector src, se::Vector dst)
	{
		se::QAngle angles;
		auto delta = src - dst;
		VectorAngles(delta, angles);
		Utils::Clamp(delta);
		return angles;
	}

	static void VectorAngles(const se::Vector& forward, se::QAngle &angles)
	{
		if (forward[1] == 0.0f && forward[0] == 0.0f)
		{
			angles[0] = (forward[2] > 0.0f) ? 270.0f : 90.0f; // Pitch (up/down)
			angles[1] = 0.0f;  //yaw left/right
		}
		else
		{
			angles[0] = atan2(-forward[2], forward.Length2D()) * -180 / 3.14159265358979323846f;
			angles[1] = atan2(forward[1], forward[0]) * 180 / 3.14159265358979323846f;

			if (angles[1] > 90) angles[1] -= 180;
			else if (angles[1] < 90) angles[1] += 180;
			else if (angles[1] == 90) angles[1] = 0;
		}
		angles[2] = 0.0f;
	}
private:
    static void Normalize(se::QAngle& angle)
    {
        while(angle.x > 89.0f) {
            angle.x -= 180.f;
        }
        while(angle.x < -89.0f) {
            angle.x += 180.f;
        }
        while(angle.y > 180.f) {
            angle.y -= 360.f;
        }
        while(angle.y < -180.f) {
            angle.y += 360.f;
        }
    }

    static void ClampAngles(se::QAngle &angles)
    {
        if(angles.y > 180.0f)
            angles.y = 180.0f;
        else if(angles.y < -180.0f)
            angles.y = -180.0f;

        if(angles.x > 89.0f)
            angles.x = 89.0f;
        else if(angles.x < -89.0f)
            angles.x = -89.0f;

        angles.z = 0;
    }
};