#include "stdafx.h"
#include "cCustomAbilityManager.h"
#include "ButtonWinProc.h"

cCustomAbilityManager::cCustomAbilityManager()
{
	MessageManager.AddListener(this, Simulator::SimulatorMessages::kMsgSaveGame);
	MessageManager.AddListener(this, Simulator::SimulatorMessages::kMsgSwitchGameMode);
	MessageManager.AddListener(this, id("CreatureAttackUsed"));

	mpLastUsedAbilities = hash_map<uint32_t, Simulator::cCreatureAbility*>();

	vector<uint32_t> abilityIDs;
	PropManager.GetPropertyListIDs(GroupIDs::CreatureAbilities,abilityIDs);
	for (int i = 0; i < abilityIDs.size(); i++)
	{
		PropertyListPtr propList;
		PropManager.GetPropertyList(abilityIDs[i], GroupIDs::CreatureAbilities, propList);

		Simulator::cCreatureAbility* ability = (Simulator::cCreatureAbility*)(new char[0x13C]);
		CALL(Address(0x004d37c0), void, Args(Simulator::cCreatureAbility*), Args(ability));//new Simulator::cCreatureAbility();
		Simulator::cCreatureAbility::Parse(ability, propList.get());

		mpAbilities.emplace(abilityIDs[i], ability);
	}
}

cCustomAbilityManager::~cCustomAbilityManager()
{
}

Simulator::cCreatureAbility* cCustomAbilityManager::GetAbility(uint32_t index)
{
	auto node = mpAbilities.find(index);
	if (node != mpAbilities.end())
	{
		//SporeDebugPrint("Found ability 0x%x!", index);
		return node->second;
	}
	return nullptr;
}

Simulator::cCreatureAbility* cCustomAbilityManager::GetLastAbilityUsed(cCreatureBasePtr creature)
{
	auto index = creature->mID;
	auto node = mpLastUsedAbilities.find(index);
	if (node != mpLastUsedAbilities.end())
	{
		return node->second;
	}
	return nullptr;
}

IAbilityStrategy* cCustomAbilityManager::GetStrategyForAbility(Simulator::cCreatureAbility* ability)
{
	auto node = mpCachedStrategies.find(ability);
	if (node != mpCachedStrategies.end())
	{
		return node->second;
	}
	
	uint32_t strategyID;
	if (App::Property::GetUInt32(ability->mpPropList.get(), id("AbilityStrategy"), strategyID))
	{
		auto strategyNode = mpAbilityStrategies.find(strategyID);
		if (strategyNode != mpAbilityStrategies.end())
		{
			IAbilityStrategy* strategy = strategyNode->second;
			mpCachedStrategies.emplace(ability, strategy);
			return strategy;
		}
	}

	return nullptr;
}

bool cCustomAbilityManager::CanUseAbility(Simulator::cCreatureAbility* ability, cCreatureBasePtr creature)
{
	uint32_t abilityIndex = -1;
	bool hasTarget = 1;
	for (int i = 0; i < creature->mpSpeciesProfile->mAbilities.size(); i++)
	{
		if (creature->GetAbility(i)->mpPropList == ability->mpPropList)
		{
			abilityIndex = i;
			break;
		}
	}

	if (abilityIndex == -1)
	{
		return false;
	}

	//auto abilityIndex = creature-
	auto strategy = GetStrategyForAbility(ability);
	hasTarget = (creature->mpCombatantTarget || (strategy && strategy->sRequiresTarget));

	auto distance = -1.0f;
	
	if (creature->mpCombatantTarget)
	{
		distance = Math::distance(creature->mPosition, creature->mpCombatantTarget->ToSpatialObject()->mPosition);
	}
	//SporeDebugPrint("distance is %f, range is %f", distance, ability->mRange);

	if (ceil(distance) < ability->mAvatarRangeMin && distance != -1.0f)
	{
		return false;
	}

	if (ability->mEnergyCost > creature->mEnergy)
	{
		return false;
	}

	if (creature->mRechargingAbilityBits[abilityIndex])
	{
		return false;
	}

	return hasTarget;
}

void cCustomAbilityManager::TriggerSkill(Simulator::cCreatureAbility* ability, cCreatureBasePtr creature)
{
	bool needsTarget = true;
	if (ability)
	{
		if (auto strategy = GetStrategyForAbility(ability))
		{
			needsTarget = strategy->sRequiresTarget;
		}
	}
	else
	{
		return;
	}

	if (creature && (creature->mpCombatantTarget || !needsTarget))
	{
		auto distance = -1.0f;
		SporeDebugPrint("%f", ability->mRushingRange);
		if (creature->mpCombatantTarget)
		{
			distance = Math::distance(creature->mPosition, creature->mpCombatantTarget->ToSpatialObject()->mPosition);
		}
		//SporeDebugPrint("distance is %f, range is %f", distance, ability->mRange);
		if (floor(distance) > ceil(ability->mRange) + creature->GetScale() && (distance > ability->mRushingRange || ability->mRushingRange == 0))
		{
			creature->WalkTo(1, creature ->mpCombatantTarget->ToSpatialObject()->mPosition, creature->mpCombatantTarget->ToSpatialObject()->mPosition.Normalized());
			return;
		}
		if (ceil(distance) < ability->mAvatarRangeMin && distance != -1.0f)
		{
			return;
		}

		if (ability->mEnergyCost > creature->mEnergy)
		{
			return;
		}

		creature->PlayAbility(ability->mpPropList->GetResourceKey().instanceID);
	}
}

void cCustomAbilityManager::CreateAbilityUI()
{
	mpAbilityUI = nullptr;
	mpAbilityUI = new UTFWin::UILayout();
	mpAbilityUI->LoadByID(id("CustomButtons"));

	for (int i = 1; i < 6; i++)
	{
		auto window = mpAbilityUI->FindWindowByID(id(("Btn" + to_string(i)).c_str()));
		window->AddWinProc(new ButtonWinProc(window, id("Adventurer_PoisonBlade1")));
	}
}


bool cCustomAbilityManager::HandleMessage(uint32_t messageID, void* message)
{
	switch (messageID)
	{
		case Simulator::SimulatorMessages::kMsgCombatantKilled:
		{
			auto castMessage = (Simulator::CombatantKilledMessage*)(message);
			if (mpLastUsedAbilities.find(castMessage->GetCombatant()->ToGameData()->mID) != mpLastUsedAbilities.end())
			{
				mpLastUsedAbilities.erase(castMessage->GetCombatant()->ToGameData()->mID);
			}
			break;
		}
		case id("CreatureAttackUsed"):
		{
			auto castMessage = (AbilityUsedData*)(message);
			mpLastUsedAbilities.emplace(castMessage->mpSourceCreature->mID, castMessage->mpAbility);
		}
	}
	// Return true if the message has been handled. Other listeners will receive the message regardless of the return value.
	return true;
}


int cCustomAbilityManager::AddRef()
{
	return DefaultRefCounted::AddRef();
}

int cCustomAbilityManager::Release()
{
	return DefaultRefCounted::Release();
}