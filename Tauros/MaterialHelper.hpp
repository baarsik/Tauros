#pragma once
#include <vector>
#include "Utils.hpp"

class MaterialHelper
{
public:
	void Initialize()
	{
		if (m_bInitialized)
			return;

		AddMaterial(
			XorStr("Solid color (Lit)"),
			XorStr("white_additive1"),
			XorStr("VertexLitGeneric"),
			XorStr("vgui/white_additive"),
			true, true, true, true);
		AddMaterial(
			XorStr("Solid color (Unlit)"),
			XorStr("white_additive3"),
			XorStr("UnlitGeneric"),
			XorStr("vgui/white_additive"),
			true, true, true, true);
		AddMaterialIgnoreZ(
			XorStr("Solid color (Lit)"),
			XorStr("white_additive2"),
			XorStr("VertexLitGeneric"),
			XorStr("vgui/white_additive"),
			true, true, true, true);
		AddMaterialIgnoreZ(
			XorStr("Solid color (Unlit)"),
			XorStr("white_additive4"),
			XorStr("UnlitGeneric"),
			XorStr("vgui/white_additive"),
			true, true, true, true);
		m_bInitialized = true;
	}

	IMaterial* GetMaterialById(int id, bool ignorez)
	{
		Initialize();
		return ignorez ? m_vMaterialsIgnoreZ[id] : m_vMaterials[id];
	}

	std::vector<std::string> GetMaterialNames(bool ignorez)
	{
		Initialize();
		return ignorez ? m_vNamesIgnoreZ : m_vNames;
	}
private:
	bool m_bInitialized = false;
	std::vector<std::string> m_vNames, m_vNamesIgnoreZ;
	std::vector<IMaterial*> m_vMaterials, m_vMaterialsIgnoreZ;

	bool AddMaterial(char* viewName, std::string name, std::string shaderName, std::string baseTexture, bool nofog, bool model, bool nocull, bool halflambert)
	{
		auto mat = CreateMaterial(name, shaderName, baseTexture, false, nofog, model, nocull, halflambert);
		if (!mat || mat->IsErrorMaterial())
			return false;

		m_vNames.push_back(viewName);
		m_vMaterials.push_back(mat);
		return true;
	}

	bool AddMaterialIgnoreZ(char* viewName, std::string name, std::string shaderName, std::string baseTexture, bool nofog, bool model, bool nocull, bool halflambert)
	{
		auto mat = CreateMaterial(name, shaderName, baseTexture, true, nofog, model, nocull, halflambert);
		if (!mat || mat->IsErrorMaterial())
			return false;

		m_vNamesIgnoreZ.push_back(viewName);
		m_vMaterialsIgnoreZ.push_back(mat);
		return true;
	}

	IMaterial* CreateMaterial(std::string name, std::string shaderName, std::string baseTexture, bool ignorez, bool nofog, bool model, bool nocull, bool halflambert) const
	{
		auto matName = XorStr("tauros_") + name;
		auto filePath = Utils::GetGameDir() + XorStr("csgo\\materials\\") + matName + XorStr(".vmt");
		if (auto file = fopen(filePath.c_str(), "r"))
		{
			fclose(file);
			return Interfaces::MaterialSystem()->FindMaterial(matName.c_str(), XorStr(TEXTURE_GROUP_MODEL));
		}

		auto materialData = XorStr("\"") + shaderName + XorStr("\"\n") +
			XorStr("{\n") +
			XorStr("\t\"$basetexture\" \"") + baseTexture + XorStr("\"\n") +
			XorStr("\t\"$ignorez\" \"") + std::to_string(ignorez) + XorStr("\"\n") +
			XorStr("\t\"$nofog\" \"") + std::to_string(nofog) + XorStr("\"\n") +
			XorStr("\t\"$model\" \"") + std::to_string(model) + XorStr("\"\n") +
			XorStr("\t\"$nocull\" \"") + std::to_string(nocull) + XorStr("\"\n") +
			XorStr("\t\"$halflambert\" \"") + std::to_string(halflambert) + XorStr("\"\n") +
			XorStr("}\n");

		if (auto file = fopen(filePath.c_str(), "w"))
		{
			fputs(materialData.c_str(), file);
			fclose(file);
			return Interfaces::MaterialSystem()->FindMaterial(matName.c_str(), XorStr(TEXTURE_GROUP_MODEL));
		}
		return nullptr;
	}
};

MaterialHelper MatHelper;