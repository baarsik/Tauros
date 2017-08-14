#pragma once

using SetClanTag_t          = void(__fastcall*)(const char*, const char*);
using IsReady_t             = void(__cdecl*)();
using ServerRankRevealAll_t = bool(__cdecl*)(int*);
using LineThroughSmoke_t    = bool(__cdecl*)(se::Vector, se::Vector, int16_t);

class SignatureHelper
{
public:
	static unsigned D3DDevice()
	{
		static auto result = **reinterpret_cast<uint32_t**>(Utils::FindSignature(XorStr("shaderapidx9.dll"), XorStr("A1 ? ? ? ? 6A 00 53")) + 0x1);
		return result;
	}

	static void SetClanTag(std::string tag, std::string name)
	{
		static auto func = reinterpret_cast<SetClanTag_t>(Utils::FindSignature(XorStr("engine.dll"), XorStr("53 56 57 8B DA 8B F9 FF 15")));
		func(tag.c_str(), name.c_str());
	}

	static void IsReady()
	{
		static auto func = reinterpret_cast<IsReady_t>(Utils::FindSignature(XorStr("client.dll"), XorStr("81 66 70 00 00 F0 FF C7 46 6C 00 00 00 00 C7 46 74 00 00 00 00 5E E9 ?? ?? ?? ?? CC CC CC CC CC CC CC CC CC CC CC CC CC 55 8B EC 83 E4 F8 83 EC 08 56 8B 35 ?? ?? ?? ?? 57")) + 0x28);
		func();
	}

	static void ServerRankRevealAll()
	{
		static auto func = reinterpret_cast<ServerRankRevealAll_t>(Utils::FindSignature(XorStr("client.dll"), XorStr("55 8B EC 8B 0D ? ? ? ? 68")));
		int param[3] = { 0, 0, 0 };
		func(param);
	}

	static bool IsBehindSmoke(se::Vector loc, se::Vector rem)
	{
		static auto func = reinterpret_cast<LineThroughSmoke_t>(Utils::FindSignature(XorStr("client.dll"), XorStr("55 8B EC 83 EC 08 8B 15 ? ? ? ? 0F 57 C0")));
		return func(loc, rem, true);
	}
};