#pragma once

#include <Spore\BasicIncludes.h>
#include "IAbilityStrategy.h"

#define cCustomAbilityManagerPtr intrusive_ptr<cCustomAbilityManager>
#define CustomAbilityManager cCustomAbilityManager::Get()[0]

struct AbilityUsedData
{
	ResourceKey mAbilityKey;
	Simulator::cCreatureAbility* mpAbility;
	cCreatureBasePtr mpSourceCreature;
	cCombatantPtr mpCreatureTarget;
};

class cCustomAbilityManager 
	: public DefaultObject
	, public App::IMessageListener
	, public DefaultRefCounted
{
public:
	static const uint32_t TYPE = id("CustomAbilityManager");
	cCustomAbilityManager();
	~cCustomAbilityManager();

	static cCustomAbilityManager* Get() {
		return object_cast<cCustomAbilityManager>(ClassManager.Create(TYPE));
	};

	//Gets an ability from the hash map.
	virtual Simulator::cCreatureAbility* GetAbility(uint32_t index);

	//Gets the last ability used by a creature.
	virtual Simulator::cCreatureAbility* GetLastAbilityUsed(cCreatureBasePtr creature);
	//Gets the strategy of an ability, using the cache if possible.
	virtual IAbilityStrategy* GetStrategyForAbility(Simulator::cCreatureAbility* ability);

	virtual bool CanUseAbility(Simulator::cCreatureAbility* ability, cCreatureBasePtr creature);
	virtual void TriggerSkill(Simulator::cCreatureAbility* ability, cCreatureBasePtr creature);
	
	virtual void CreateAbilityUI();


	bool HandleMessage(uint32_t messageID, void* message) override;


	virtual int AddRef() override;
	virtual int Release() override;

private:
	hash_map<uint32_t, Simulator::cCreatureAbility*> mpLastUsedAbilities;
	hash_map<uint32_t, IAbilityStrategy*> mpAbilityStrategies;
	hash_map<Simulator::cCreatureAbility*, IAbilityStrategy*> mpCachedStrategies;
	hash_map<uint32_t, Simulator::cCreatureAbility*> mpAbilities;
	UILayoutPtr mpAbilityUI;
};
