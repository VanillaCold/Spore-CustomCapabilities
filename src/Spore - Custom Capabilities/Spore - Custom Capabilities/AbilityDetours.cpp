#include "stdafx.h"
#include "Detours.h"
#include <Spore/Simulator/SubSystem/TerraformingManager.h>
//#include "cCapabilityManager.h"
#include "cCustomAbilityManager.h"

int TakeDamageDetour::DETOUR(float damage, uint32_t attackerPoliticalID, int unk, const Vector3& unkPos, cCombatant* pAttacker)
{
	//Get the attacker, and verify it's the space-stage.
	if (pAttacker)
	{
		//Cast the attacker to a creature.
		auto creature = object_cast<Simulator::cCreatureBase>(pAttacker);
		if (creature) //if it is one,
		{
			//Get the current ability index being used,
			uint32_t attackIndex = creature->mCurrentAttackIdx;
			//and get the ability from that.
			auto ability = creature->GetAbility(attackIndex);
			//If it's a valid ability,
			if (ability)
			{
				//DO STUFF
			}
			//else, if it's not valid, fall back on the CustomAbilityManager.
			else
			{
				//get the ability,
				ability = CustomAbilityManager.GetLastAbilityUsed(creature);
				if (ability)
				{
					//DO STUFF
				}
			}
		}
	}
	SporeDebugPrint("Damage is %f, caused by political ID of 0x%x", damage, attackerPoliticalID);
	return original_function(this, damage, attackerPoliticalID, unk, unkPos, pAttacker);
}

void PlayAbilityDetour::DETOUR(int abilityIndex, Anim::AnimIndex* dstAnimIndex)
{
	AbilityUsedData* data = new AbilityUsedData();
	uint32_t index = abilityIndex;
	auto ability = this->GetAbility(index);
	if (ability != nullptr)
	{
		data->mAbilityKey = ability->mpPropList->GetResourceKey();
		data->mpAbility = ability;
		data->mpCreatureTarget = this->mpCombatantTarget;
		data->mpSourceCreature = this;
		MessageManager.MessageSend(id("CreatureAttackUsed"), data);

		if ((uint32_t)index >= (uint32_t)this->GetAbilitiesCount())
		{
			bool hasBeenGiven = false;
			for (int i = this->GetAbilitiesCount() - 1; i >= 0; i--)
			{
				if (this->mpSpeciesProfile->mAbilities[i]->mpPropList->GetResourceKey().instanceID == index)
				{
					hasBeenGiven = true;
					index = i;
					break;
				}
			}
			if (hasBeenGiven == false)
			{
				this->mpSpeciesProfile->mAbilities.push_back(ability);
				index = this->mpSpeciesProfile->mAbilities.size() - 1;
				this->mRechargingAbilityBits[index] = 0;
				this->mInUseAbilityBits[index] = 0;
				auto a = Simulator::cAbilityState(this->mAbilityStates[index - 1]);
				a.field_08 = ResourceID(this->mAbilityStates[index - 1].field_08);
				a.field_08.instanceID = abilityIndex;
				this->mAbilityStates.push_back(a);
			}
		}
		SporeDebugPrint("ability index is 0x%x, name is %ls, count is %i", index, ability->nName.GetText(), this->GetAbilitiesCount());
	}
	original_function(this, index, dstAnimIndex);
}

Simulator::cCreatureAbility* GetAbilityDetour::DETOUR(int index)
{
	if (index > this->GetAbilitiesCount())
	{
		for (auto ability : this->mpSpeciesProfile->mAbilities)
		{
			if (ability->mpPropList->GetResourceKey().instanceID == index)
			{
				SporeDebugPrint("0x%x, 0x%x", index, ability->mpPropList->GetResourceKey().instanceID);
				return ability;
			}
		}
	}

	auto ret = original_function(this, index);
	
	if (ret == nullptr && index != 0xFFFFFFFF && this != nullptr)
	{
		//ModAPI::Log("detoured ability func, arg is 0x%x", index);
		uint32_t number = (uint32_t)index;
		if (cCustomAbilityManager::Get() != nullptr)
		{
			auto ability = CustomAbilityManager.GetAbility(number);
			if (ability != nullptr)
			{
				return ability;
			}
		}
		//return original_function(this, number);
	}
	return ret;
}