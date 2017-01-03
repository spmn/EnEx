#include "Main.h"

CSettings::stConfig CSettings::Config;

CSimpleIniA CSettings::m_ConfigIni;
CSimpleIniA CSettings::m_DoorsIni;

void CSettings::LoadConfig()
{
	bool create = false;

	if (m_ConfigIni.LoadFile("SAMPFUNCS\\EnEx_Config.ini") < 0)
	{
		SF->Log("[ENEX] Couldn't find configuration file/invalid file. It is being recreated.");
		create = true;
	}

	Config.CmdEnter = m_ConfigIni.GetValue("commands", "enter", "/enter");
	Config.CmdExit = m_ConfigIni.GetValue("commands", "exit", "/exit");
	
	Config.Keys[0] = m_ConfigIni.GetLongValue("combo_keys", "key_0", 0x46); // F key
	Config.Keys[1] = m_ConfigIni.GetLongValue("combo_keys", "key_1", 0);

	if (Config.Keys[0] == 0)
	{
		SF->Log("[ENEX] Invalid config. \"key_0\" can't be 0.");
		SF->Log("[ENEX] Setting \"key_0\" to F key.");
		Config.Keys[0] = 0x46;

		create = true;
	}

	if(create)
		SaveConfig();
}

void CSettings::SaveConfig()
{
	m_ConfigIni.SetValue("commands", "enter", Config.CmdEnter.c_str());
	m_ConfigIni.SetValue("commands", "exit", Config.CmdExit.c_str());

	m_ConfigIni.SetLongValue("combo_keys", "key_0", Config.Keys[0], "; See: https://msdn.microsoft.com/en-us/library/windows/desktop/dd375731(v=vs.85).aspx", true);
	m_ConfigIni.SetLongValue("combo_keys", "key_1", Config.Keys[1], "; If \"key_1\" is set to \"0x00\", then only \"key_0\" has to be pressed.", true);

	m_ConfigIni.SaveFile("SAMPFUNCS\\EnEx_Config.ini");
}

void CSettings::LoadDoors()
{
	if (m_DoorsIni.LoadFile("SAMPFUNCS\\EnEx_Doors.ini") < 0)
		return SF->Log("[ENEX] Couldn't find doors file/invalid file."), void();

	CSimpleIniA::TNamesDepend sections;
	m_DoorsIni.GetAllSections(sections);
	
	for (auto& section : sections)
	{
		int doortype;
		CVector pos;
		float range;
		std::string description;

		doortype = m_DoorsIni.GetLongValue(section.pItem, "doortype");
		pos.fX = static_cast<float>(m_DoorsIni.GetDoubleValue(section.pItem, "pos_x"));
		pos.fY = static_cast<float>(m_DoorsIni.GetDoubleValue(section.pItem, "pos_y"));
		pos.fZ = static_cast<float>(m_DoorsIni.GetDoubleValue(section.pItem, "pos_z"));
		range = static_cast<float>(m_DoorsIni.GetDoubleValue(section.pItem, "range"));
		description = m_DoorsIni.GetValue(section.pItem, "description");

		new CEnEx(doortype, pos, range, description, section.pItem, true);
	}
}

void CSettings::AddDoor(CEnEx *const door)
{
	m_DoorsIni.SetLongValue(door->GetIniSection().c_str(), "doortype", door->GetDoorType());
	m_DoorsIni.SetDoubleValue(door->GetIniSection().c_str(), "pos_x", door->GetPosition().fX);
	m_DoorsIni.SetDoubleValue(door->GetIniSection().c_str(), "pos_y", door->GetPosition().fY);
	m_DoorsIni.SetDoubleValue(door->GetIniSection().c_str(), "pos_z", door->GetPosition().fZ);
	m_DoorsIni.SetDoubleValue(door->GetIniSection().c_str(), "range", door->GetRange());
	m_DoorsIni.SetValue(door->GetIniSection().c_str(), "description", door->GetDescription().c_str());

	m_DoorsIni.SaveFile("SAMPFUNCS\\EnEx_Doors.ini");
}

void CSettings::RemoveDoor(CEnEx *const door)
{
	m_DoorsIni.Delete(door->GetIniSection().c_str(), NULL);
	m_DoorsIni.SaveFile("SAMPFUNCS\\EnEx_Doors.ini");
}