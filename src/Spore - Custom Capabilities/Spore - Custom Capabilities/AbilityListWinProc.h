#pragma once

#include <Spore\BasicIncludes.h>
#include "ButtonWinProc.h"

#define AbilityListWinProcPtr intrusive_ptr<AbilityListWinProc>

// To avoid repeating UTFWin:: all the time.
using namespace UTFWin;

class AbilityListWinProc 
	: public IWinProc
	, public DefaultRefCounted
{
public:
	static const uint32_t TYPE = id("AbilityListWinProc");
	
	AbilityListWinProc(int abilityIndex, UTFWin::IWindow* parent, ButtonWinProc* btnProc);
	~AbilityListWinProc();

	int AddRef() override;
	int Release() override;
	void* Cast(uint32_t type) const override;
	
	int GetEventFlags() const override;
	// This is the function you have to implement, called when a window you added this winproc to received an event
	bool HandleUIMessage(IWindow* pWindow, const Message& message) override;
	
	int abilityIndex;
	IWindowPtr mpWindowOwner;
	ButtonWinProc* mpOwnerProc;

	static bool sbRemove;

};
