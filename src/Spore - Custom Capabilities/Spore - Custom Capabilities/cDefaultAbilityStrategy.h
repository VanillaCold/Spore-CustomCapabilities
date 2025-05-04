#pragma once

#include <Spore\BasicIncludes.h>
#include "IAbilityStrategy.h";

#define cDefaultAbilityStrategyPtr intrusive_ptr<cDefaultAbilityStrategy>

class cDefaultAbilityStrategy 
	: public IAbilityStrategy
{
public:
	static const uint32_t TYPE = id("cDefaultAbilityStrategy");



	virtual void OnPlayerPreUse(Simulator::cCreatureAbility* ability);
	virtual void OnUse(cCreatureBasePtr source, Simulator::cCreatureAbility* ability);
	virtual void OnHit(cCreatureBasePtr source, cCombatantPtr target, Simulator::cCreatureAbility* ability, float damage);
	virtual void CanUse(cCreatureBasePtr source, Simulator::cCreatureAbility* ability);

	cDefaultAbilityStrategy();
	~cDefaultAbilityStrategy();
};
