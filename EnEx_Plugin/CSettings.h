#pragma once

#include "Main.h"

class CSettings
{
public:
	struct stConfig
	{
		std::string CmdEnter;
		std::string CmdExit;
		int Keys[2];
	};
	static stConfig Config;
	
	static void LoadConfig();
	static void SaveConfig();
	
	static void LoadDoors();
	static void AddDoor(CEnEx *const door);
	static void RemoveDoor(CEnEx *const door);

private:
	static CSimpleIniA m_ConfigIni;
	static CSimpleIniA m_DoorsIni;
};

