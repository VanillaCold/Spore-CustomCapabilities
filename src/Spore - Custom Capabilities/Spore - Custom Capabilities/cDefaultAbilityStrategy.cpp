#include "stdafx.h"
#include "cDefaultAbilityStrategy.h"
#include "cCustomAbilityManager.h"

void cDefaultAbilityStrategy::OnPlayerPreUse(Simulator::cCreatureAbility* ability)
{
	auto avatar = GameNounManager.GetAvatar();
	int index = 0;
	for (int i = 0; i < avatar->GetAbilitiesCount(); i++)
	{
		if (avatar->GetAbility(i)->mpPropList->GetResourceKey().instanceID == ability->mpPropList->GetResourceKey().instanceID)
		{
			index = i;
			break;
		}
	}
	if (index != -1)
	{
		STATIC_CALL(Address(0x00d49560), void, Args(int), Args(index));
	}
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

