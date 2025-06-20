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

	// Triggered when the player uses the ability via the UI buttons.
	// can be used to implement your own logic.
	virtual void OnPlayerPreUse(Simulator::cCreatureAbility* ability) = 0;
	// Return value determines whether the ability's usage will be cancelled.
	// For vanilla combat abilities this will be triggered when cCreatureBase::PlayAbility is used.
	virtual bool OnUse(cCreatureBasePtr source, Simulator::cCreatureAbility* ability) = 0;
	// Triggered when the ability hits a target.
	// Only in use when PlayAbility is used, and the ability deals damage.
	// An AbilityUsedData struct should be sent with the "CreatureAttackUsed" message via the MesssageManager for this to be hit automatically.
	// alternatively, for projectiles, this can be implemented and called manually.
	virtual void OnHit(cCreatureBasePtr source, cCombatantPtr target, Simulator::cCreatureAbility* ability, float damage) = 0;
	// Determines whether it will appear as selectable or not in the UI.
	// clicking it can still do someting depending on what you do in OnPlayerPreUse.
	// Currently will only affect players - won't stop the AI unless you do something in OnUse().
	virtual bool CanUse(cCreatureBasePtr source, Simulator::cCreatureAbility* ability) = 0;

	static bool sRequiresTarget;

	int AddRef() override;
	int Release() override;
	void* Cast(uint32_t type) const override;
};
