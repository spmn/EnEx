#pragma once

#include "Main.h"

class CEnEx
{
public:
	enum DoorTypes {
		DOOR_TYPE_ENTRANCE,
		DOOR_TYPE_EXIT,
		NUMBER_OF_DOOR_TYPES,
		INVALID_DOOR_TYPE = NUMBER_OF_DOOR_TYPES
	};
	static const std::string DoorTypes_Names[];

	CEnEx(BYTE doortype, CVector pos, float range = 3.0, std::string description = "EnEx Door", std::string inisection = "Unknown", bool loaded = false);
	~CEnEx();
	
	void Use();

	static CEnEx *FindClosestDoor();
	
	static void CALLBACK CMD_Add(std::string params);
	static void CALLBACK CMD_Remove(std::string params);
	static void CALLBACK CMD_Info(std::string params);

	static std::vector<CEnEx*> *GetEnExVec();

	// (G/S)etters
	inline void SetDoorType(BYTE doortype) { m_DoorType = doortype; }
	inline BYTE GetDoorType() const { return m_DoorType; }
	inline void SetRange(float range) { m_Range = range; }
	inline float GetRange() const { return m_Range; }
	inline void SetPosition(CVector pos) { m_Pos = pos; }
	inline CVector GetPosition() const { return m_Pos; }
	inline void SetDescription(std::string desc) { m_Description = desc; }
	inline std::string GetDescription() const { return m_Description; }
	inline void SetIniSection(std::string section) { m_IniSection = section; }
	inline std::string GetIniSection() const { return m_IniSection; }

private:
	BYTE m_DoorType;
	float m_Range;
	CVector m_Pos;
	std::string m_Description;
	std::string m_IniSection;
};

