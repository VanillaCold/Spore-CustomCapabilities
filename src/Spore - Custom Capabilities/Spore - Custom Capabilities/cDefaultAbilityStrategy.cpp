#include "stdafx.h"
#include "cDefaultAbilityStrategy.h"

void cDefaultAbilityStrategy::OnPlayerPreUse(Simulator::cCreatureAbility* ability)
{
}

bool cDefaultAbilityStrategy::OnUse(cCreatureBasePtr source, Simulator::cCreatureAbility* ability)
{
	return true;
}

void cDefaultAbilityStrategy::OnHit(cCreatureBasePtr source, cCombatantPtr target, Simulator::cCreatureAbility* ability, float damage)
{
}

bool cDefaultAbilityStrategy::CanUse(cCreatureBasePtr source, Simulator::cCreatureAbility* ability)
{
	return true;
}

cDefaultAbilityStrategy::cDefaultAbilityStrategy()
{
}


cDefaultAbilityStrategy::~cDefaultAbilityStrategy()
{
}

