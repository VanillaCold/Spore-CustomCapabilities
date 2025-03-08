// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include "CapabilityFactory.cpp";
#include "Detours.h"
#include "cCustomAbilityManager.h"
#include "CustomAbilityFactory.cpp";
#include "ShowUICheat.h"

CapabilityFactory factory;
cCustomAbilityFactory factory2;

//static bool overrideCount = false;

void Initialize() {
	ClassManager.AddFactory(&factory);
	ClassManager.AddFactory(&factory2);
	
	cCustomAbilityManager::Get();
	CheatManager.AddCheat("ShowUI", new ShowUICheat());
}

void Dispose()
{
	factory.sInstance = nullptr;
	factory2.sInstance = nullptr;

	auto test = cCustomAbilityManager::Get();
	SporeDebugPrint("%x", test->TYPE);
	// This method is called when the game is closing
}

//FUN_005e4de0
static_detour(SomethingOrOtherDetour, uint32_t(int*, uint32_t))
{
	uint32_t detoured(int* somethingID, uint32_t capID)
	{
		if (capID == id("stupidity"))
		{
			return capID;
		}
		return original_function(somethingID, capID);
	}
};

static_detour(AbilityCountDetour, int(uint32_t, int))
{
	uint32_t detoured(uint32_t capabilityID, int level)
	{
		if (capabilityID == id("modelCapabilityStupid"))//0x02166464)
		{
			return 0xBB85808A;
		}
		return original_function(capabilityID, level);
	}
	//FUN_00459840
};

member_detour(GetIndexDetour, Simulator::cCreatureBase, uint32_t(uint32_t, bool recharging, uint32_t))
{
	uint32_t detoured(uint32_t param0, bool recharging, uint32_t param2)
	{
		auto out = original_function(this, param0, recharging, param2);
		if ((int)out == -1 && CustomAbilityManager.GetAbility(param0))
		{
			original_function(this, param0, 0, 0);
			return param0;
		}
		return out;
	}
};

member_detour(AnotherTestDetour, Simulator::cCreatureBase, void(uint32_t))
{
	//FUN_00c07500
	void detoured(uint32_t param2)
	{
		if (param2 > this->GetAbilitiesCount())
		{
			//SporeDebugPrint("ohoho hold on");
			this->mCurrentAttackIdx = param2;
		}
		return original_function(this, param2);
	}
};



member_detour(YetAnotherTest, Simulator::cCreatureBase, void(uint32_t))
{
	//FUN_00c07500
	//actually no, detour FUN_00c19900 instead.
	void detoured(uint32_t param1)
	{
		SporeDebugPrint("aaa");
		return original_function(this, param1);
	}
};

member_detour(ContinueAbilityDetour, Simulator::cCreatureBase, void(float))
{
	void detoured(float param1)
	{
		if (this->mCurrentAttackIdx > this->GetAbilitiesCount())
		{
			//overrideCount = true;
			original_function(this, param1);
			//overrideCount = false;
		}
		else
		{
			return original_function(this, param1);
		}
	}
};

virtual_detour(GetAbilityCountDetour, Simulator::cCreatureAnimal, Simulator::cCreatureBase, uint32_t())
{
	uint32_t detoured()
	{
		//if (overrideCount == true)
		//{
		//	overrideCount = false;
		//	return UINT_MAX;
		//
		//else
		//{
			return original_function(this);
		//}
	}
};

void AttachDetours()
{
	//00c0e7e0

	//ContinueAbilityDetour::attach(Address(0x00c1e8b0));

	GetAbilityDetour::attach(Address(0x00c047d0));

	//AbilityCountDetour::attach(Address(0x00459840));

	//SomethingOrOtherDetour::attach(Address(0x005e4de0));

	PlayAbilityDetour::attach(GetAddress(Simulator::cCreatureBase, PlayAbility));

	TakeDamageDetour::attach(Address(0x00bfcf10));

	//GetIndexDetour::attach(Address(0x00c19800));

	//AnotherTestDetour::attach(Address(0x00c07500));

	//GetAbilityCountDetour::attach(Address(0x00c04750));
	
	//YetAnotherTest::attach(Address(0x00c19900));//0x00c02b40));

	//TestDetourAAAAAAAA::attach(Address(0x00459840));
	// Call the attach() method on any detours you want to add
	// For example: cViewer_SetRenderType_detour::attach(GetAddress(cViewer, SetRenderType));
}


// Generally, you don't need to touch any code here
BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		ModAPI::AddPostInitFunction(Initialize);
		ModAPI::AddDisposeFunction(Dispose);

		PrepareDetours(hModule);
		AttachDetours();
		CommitDetours();
		break;

	case DLL_PROCESS_DETACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
		break;
	}
	return TRUE;
}

