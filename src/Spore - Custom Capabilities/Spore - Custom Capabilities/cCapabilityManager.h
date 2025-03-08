#pragma once

#include <Spore\BasicIncludes.h>

#define cCapabilityManagerPtr intrusive_ptr<cCapabilityManager>
#define CapabilityManager cCapabilityManager::Get()[0]

struct CreatureCapability
{
	CreatureCapability(uint32_t id, int newLevel)
	{
		capabilityID = id;
		level = newLevel;
	}
	uint32_t capabilityID;
	int level;
};


class cCapabilityManager 
	: public DefaultObject
{
public:
	static const uint32_t TYPE = id("CapabilityManager");
	

	static cCapabilityManager* Get() {
		return object_cast<cCapabilityManager>(ClassManager.Create(TYPE));
	}

	virtual CreatureCapability GetCapability(ResourceKey creatureKey, uint32_t capabilityID);
	virtual Simulator::cCreatureAbility* GetAbilityForCreature(uint32_t capabilityID);

	void* Cast(uint32_t type) const override;


private:

	virtual CreatureCapability GenerateCapability(ResourceKey creatureKey, uint32_t capabilityID);
	hash_map<ResourceKey, map<uint32_t, CreatureCapability>> mpCreatureCapabilities;

};