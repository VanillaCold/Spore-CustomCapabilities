#include "stdafx.h"
#include "cCapabilityManager.h"

CreatureCapability cCapabilityManager::GetCapability(ResourceKey creatureKey, uint32_t capabilityID)
{
	auto capabilityList = mpCreatureCapabilities.find(creatureKey);
	if (capabilityList != mpCreatureCapabilities.end())
	{
		SporeDebugPrint("found cached creature...");
		map<uint32_t, CreatureCapability> capabilityMap = mpCreatureCapabilities.find(creatureKey).get_node()->mValue.second;
		
		auto capabilityNode = capabilityMap.find(capabilityID);
		if (capabilityNode != capabilityMap.end())
		{
			SporeDebugPrint("found cached capability!");
			return capabilityNode.mpNode->mValue.second;
		}
	}
	return GenerateCapability(creatureKey, capabilityID);
}

Simulator::cCreatureAbility* cCapabilityManager::GetAbilityForCreature(uint32_t capabilityID)
{
	return nullptr;
}

CreatureCapability cCapabilityManager::GenerateCapability(ResourceKey creatureKey, uint32_t capabilityID)
{
	SporeDebugPrint("start!");
	auto capabilityListNode = mpCreatureCapabilities.find(creatureKey);
	bool pushCapability;
	map<uint32_t, CreatureCapability>& capabilityList = map<uint32_t, CreatureCapability>();
	if (capabilityListNode == mpCreatureCapabilities.end())
	{
		//push to thingy :D
		mpCreatureCapabilities.emplace(creatureKey, capabilityList);
	}
	else
	{
		capabilityList = capabilityListNode.get_node()->mValue.second;
	}
	ResourceObjectPtr res;
	if (ResourceManager.GetResource(creatureKey, &res))
	{
		cEditorResourcePtr creatureResource = object_cast<Editors::cEditorResource>(res);
		if (creatureResource != nullptr)
		{
			auto blocks = creatureResource->mBlocks;
			int maxCapability = 0;
			for each (Editors::cEditorResourceBlock block in blocks)
			{
				PropertyListPtr propList;
				if (PropManager.GetPropertyList(block.instanceID, block.groupID, propList))
				{
					int capabilityLevel;
					if (App::Property::GetInt32(propList.get(), capabilityID, capabilityLevel))
					{
						maxCapability = max(maxCapability, capabilityLevel);
					}
				}
			}
			CreatureCapability capability = CreatureCapability(capabilityID, maxCapability);

			if (capabilityListNode !=mpCreatureCapabilities.end())
			{
				map<uint32_t, CreatureCapability> capabilityList = capabilityListNode.get_node()->mValue.second;
				capabilityList.emplace(capabilityID, capability);
				mpCreatureCapabilities.erase(creatureKey);
				mpCreatureCapabilities.emplace(creatureKey, capabilityList);
			}
			else
			{
				map<uint32_t, CreatureCapability> capabilityList = map<uint32_t, CreatureCapability>();
				capabilityList.emplace(capabilityID, capability);
				mpCreatureCapabilities.emplace(creatureKey, capabilityList);
			}


			
			return capability;
		}
	}

	return CreatureCapability(0,0);
}


// You can extend this function to return any other types your class implements.
void* cCapabilityManager::Cast(uint32_t type) const
{
	CLASS_CAST(Object);
	CLASS_CAST(cCapabilityManager);
	return nullptr;
}