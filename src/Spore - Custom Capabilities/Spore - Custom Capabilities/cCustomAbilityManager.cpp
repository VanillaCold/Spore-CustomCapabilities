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
	auto propList = ability->mpPropList;
	auto node = mpCachedStrategies.find(propList->GetResourceKey());
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
			mpCachedStrategies.emplace(propList->GetResourceKey(), strategy);
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
	if (CustomAbilityManager.CanUseAbility(ability, creature))
	{
		/*int abilityIndex = -1;
		for (int i = 0; i < creature->GetAbilitiesCount(); i++)
		{
			auto newAbility = creature->GetAbility(i);
			if (newAbility->mpPropList->GetResourceKey().instanceID == ability->mpPropList->GetResourceKey().instanceID)
			{
				abilityIndex = i;
				break;
			}
		}

		if (abilityIndex >= 0)
		{
			// Call the function that prompts the game to do funny stuff, essentially.
			
			//STATIC_CALL(Address(0x00d49560), void, Args(int), Args(abilityIndex));
		}*/
		GetStrategyForAbility(ability)->OnPlayerPreUse(ability);
	}
}

void cCustomAbilityManager::CreateAbilityUI()
{
	

	if (WindowManager.GetMainWindow()->FindWindowByID(0x01012032))
	{
		const vector<uint32_t> vec = { 0x01012031, 0x01012032, 0x01012033, 0x01012034, 0x01012030, 0x01012011, 0x01012012, 0x01012013, 0x01012010, 0x01012020, 0x01012021, 0x01012022, 0x01012023 };
		for (auto winID : vec)
		{
			auto a = WindowManager.GetMainWindow()->FindWindowByID(winID);
			
			/*auto layout = new UTFWin::UILayout();
			layout->LoadByID(id("btn"));
			layout->SetVisible(true);
			layout->SetParentWindow(a->GetParent());
			auto newWindow = layout->FindWindowByID(0x01012020);

			newWindow->SetArea(a->GetArea());
			newWindow->SetDrawable(a->GetDrawable());
			newWindow->SetControlID(a->GetControlID());
			a->GetParent()->BringToFront(newWindow);
			

			a->GetParent()->RemoveWindow(a);*/

			a->AddWinProc(new ButtonWinProc(a, 0x0));
		}
	}
	else
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
}

void cCustomAbilityManager::AddStrategy(uint32_t strategyID, IAbilityStrategy* strategy)
{
	mpAbilityStrategies.emplace(strategyID, strategy);
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