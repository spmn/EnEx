#include "Main.h"

SAMPFUNCS *SF = new SAMPFUNCS();

void CALLBACK MainLoop()
{
	static bool init = false;
	if (!init)
	{
		if (GAME == nullptr)
			return;
		if (GAME->GetSystemState() != eSystemState::GS_PLAYING_GAME)
			return;
		if (!SF->getSAMP()->IsInitialized())
			return;

		std::srand(static_cast<unsigned int>(std::time(0)));
		
		CSettings::LoadConfig();
		CSettings::LoadDoors();

		SF->getSAMP()->registerChatCommand("enex_add", CEnEx::CMD_Add);
		SF->getSAMP()->registerChatCommand("enex_remove", CEnEx::CMD_Remove);
		SF->getSAMP()->registerChatCommand("enex_info", CEnEx::CMD_Info);

		SF->Log("[ENEX] (En)trance/(Ex)it loaded!");
		SF->Log("[ENEX] Author: Spmn");
		SF->Log("[ENEX] Commands: /enex_add - /enex_remove - /enex_info");
		init = true;
	}
	else
	{
		static DWORD lastKeyPress = 0;
		if (GetTickCount() - lastKeyPress > 1000 && GetAsyncKeyState(CSettings::Config.Keys[0]) & 0x8000 && (CSettings::Config.Keys[1] == 0 || GetAsyncKeyState(CSettings::Config.Keys[1]) & 0x8000))
		{
			lastKeyPress = GetTickCount();
			CEnEx *EnEx = CEnEx::FindClosestDoor();
			if (EnEx != nullptr)
			{
				EnEx->Use();
			}
		}
	}
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD dwReasonForCall, LPVOID lpReserved)
{
	switch (dwReasonForCall)
	{
		case DLL_PROCESS_ATTACH:
			SF->initPlugin(MainLoop, hModule);
			break;
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			break;
	}
	return TRUE;
}
