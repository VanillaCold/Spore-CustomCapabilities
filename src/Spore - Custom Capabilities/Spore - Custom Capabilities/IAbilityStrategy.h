#pragma once

#include <Spore\BasicIncludes.h>

#define IAbilityStrategyPtr intrusive_ptr<IAbilityStrategy>

class IAbilityStrategy 
	: public Object
	, public DefaultRefCounted
{
public:
	static const uint32_t TYPE = id("IAbilityStrategy");
	
	IAbilityStrategy();
	~IAbilityStrategy();

	virtual void OnUse(cCreatureBasePtr source, Simulator::cCreatureAbility* ability) = 0;
	virtual void OnHit(cCreatureBasePtr source, cCombatantPtr target, Simulator::cCreatureAbility* ability, float damage) = 0;

	static bool sRequiresTarget;

	int AddRef() override;
	int Release() override;
	void* Cast(uint32_t type) const override;
};
