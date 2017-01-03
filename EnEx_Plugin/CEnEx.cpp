#include "Main.h"

const std::string CEnEx::DoorTypes_Names[] = {
	"entrance",
	"exit"
};
static_assert(sizeof(CEnEx::DoorTypes_Names) != CEnEx::DoorTypes::NUMBER_OF_DOOR_TYPES, "DoorTypes mismatch");

CEnEx::CEnEx(BYTE doortype, CVector pos, float range, std::string description, std::string inisection, bool loaded)
{
	m_DoorType = doortype;
	m_Pos = pos;
	m_Range = range;
	m_Description = description;
	
	if (loaded)
	{
		m_IniSection = inisection;
	}
	else
	{
		m_IniSection = description;
		std::replace(m_IniSection.begin(), m_IniSection.end(), ' ', '_');
		m_IniSection += "_" + std::to_string(std::rand());
		CSettings::AddDoor(this);
	}

	GetEnExVec()->push_back(this);
}


CEnEx::~CEnEx()
{
	auto vec = GetEnExVec();
	vec->erase(std::remove(vec->begin(), vec->end(), this), vec->end());

	CSettings::RemoveDoor(this);
}

void CEnEx::Use()
{
	switch (this->m_DoorType)
	{
	case DOOR_TYPE_ENTRANCE:
		SF->getSAMP()->getPlayers()->pLocalPlayer->Say(const_cast<char *>(CSettings::Config.CmdEnter.c_str()));
		break;
	case DOOR_TYPE_EXIT:
		SF->getSAMP()->getPlayers()->pLocalPlayer->Say(const_cast<char *>(CSettings::Config.CmdExit.c_str()));
		break;
	}
}

CEnEx *CEnEx::FindClosestDoor()
{
	actor_info* actorInfo = SF->getSAMP()->getPlayers()->pLocalPlayer->pSAMP_Actor->pGTA_Ped;
	if (actorInfo == nullptr) 
		return nullptr;
	
	CPed *ped = GAME->GetPools()->GetPed((DWORD*)actorInfo);
	if (ped == nullptr)
		return nullptr;
	
	CVector *pedPos = ped->GetPosition();
	if (pedPos == nullptr)
		return nullptr;

	auto vec = GetEnExVec();
	auto mindist_door = std::min_element(vec->begin(), vec->end(),
		[pedPos](CEnEx* const& lhs, CEnEx* const& rhs) { return (lhs->GetPosition() - *pedPos).LengthSquared() < (rhs->GetPosition() - *pedPos).LengthSquared(); });

	if (mindist_door == vec->end())
		return nullptr;

	CEnEx *door = *mindist_door;
	CVector VectDist(door->GetPosition() - *pedPos);
	if (VectDist.Length() > door->m_Range)
		return nullptr;

	return door;
}

void CALLBACK CEnEx::CMD_Add(std::string params)
{
	std::string
		str_doortype,
		str_desc = "",
		str_temp;
	BYTE doortype;
	float range = -1.0;

	std::stringstream ss(params);
	ss >> str_doortype >> range;
	while (ss >> str_temp)
		str_desc += str_temp + ' ';
	if (!str_desc.empty())
		str_desc.pop_back();
	std::transform(str_doortype.begin(), str_doortype.end(), str_doortype.begin(), ::tolower);

	// validate parameters
	for (doortype = 0; doortype < CEnEx::DoorTypes::NUMBER_OF_DOOR_TYPES; ++doortype)
	{
		if (CEnEx::DoorTypes_Names[doortype] == str_doortype)
			break;
	}
	if (doortype == CEnEx::DoorTypes::NUMBER_OF_DOOR_TYPES || range < 0.0 || str_desc.length() < 1)
		return SF->getSAMP()->getChat()->AddChatMessage(-1, "[ENEX] Use: /enex_add [door type(entrance/exit)] [range] [description]"), void();

	actor_info* actorInfo = SF->getSAMP()->getPlayers()->pLocalPlayer->pSAMP_Actor->pGTA_Ped;
	if (actorInfo == nullptr)
		return;

	CPed *ped = GAME->GetPools()->GetPed((DWORD*)actorInfo);
	if (ped == nullptr)
		return;

	CVector *pedPos = ped->GetPosition();
	if (pedPos == nullptr)
		return;

	CEnEx *EnEx = new CEnEx(doortype, *pedPos, range, str_desc);
	if (EnEx == nullptr)
		return;

	SF->getSAMP()->getChat()->AddChatMessage(-1, "[ENEX] Added %s \"%s\". Pos = (%.1f, %.1f, %.f), range = %.1f, IniSection = \"%s\"", str_doortype.c_str(), str_desc.c_str(), pedPos->fX, pedPos->fY, pedPos->fZ, range, EnEx->GetIniSection().c_str());
}

void CALLBACK CEnEx::CMD_Remove(std::string params)
{
	CEnEx *EnEx = CEnEx::FindClosestDoor();
	if (EnEx == nullptr)
		return SF->getSAMP()->getChat()->AddChatMessage(-1, "[ENEX] No door found here."), void();

	SF->getSAMP()->getChat()->AddChatMessage(-1, "[ENEX] Deleted %s \"%s\". Pos = (%.1f, %.1f, %.f), range = %.1f, IniSection = \"%s\"", CEnEx::DoorTypes_Names[EnEx->GetDoorType()].c_str(), EnEx->GetDescription().c_str(), EnEx->GetPosition().fX, EnEx->GetPosition().fY, EnEx->GetPosition().fZ, EnEx->GetRange(), EnEx->GetIniSection().c_str());
	delete EnEx;
}

void CALLBACK CEnEx::CMD_Info(std::string params)
{
	CEnEx *EnEx = CEnEx::FindClosestDoor();
	if (EnEx == nullptr)
		return SF->getSAMP()->getChat()->AddChatMessage(-1, "[ENEX] No door found here."), void();

	SF->getSAMP()->getChat()->AddChatMessage(-1, "[ENEX] Info %s: \"%s\". Pos = (%.1f, %.1f, %.f), range = %.1f, IniSection = \"%s\"", CEnEx::DoorTypes_Names[EnEx->GetDoorType()].c_str(), EnEx->GetDescription().c_str(), EnEx->GetPosition().fX, EnEx->GetPosition().fY, EnEx->GetPosition().fZ, EnEx->GetRange(), EnEx->GetIniSection().c_str());
}

std::vector<CEnEx*> *CEnEx::GetEnExVec()
{
	static std::vector<CEnEx*> vec_EnEx;
	return &vec_EnEx;
}