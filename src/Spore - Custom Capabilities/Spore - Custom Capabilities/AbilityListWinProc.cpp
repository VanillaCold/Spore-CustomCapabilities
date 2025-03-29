#include "stdafx.h"
#include "AbilityListWinProc.h"

AbilityListWinProc::AbilityListWinProc()
{
}


AbilityListWinProc::~AbilityListWinProc()
{
}

// For internal use, do not modify.
int AbilityListWinProc::AddRef()
{
	return DefaultRefCounted::AddRef();
}

// For internal use, do not modify.
int AbilityListWinProc::Release()
{
	return DefaultRefCounted::Release();
}

// You can extend this function to return any other types your class implements.
void* AbilityListWinProc::Cast(uint32_t type) const
{
	CLASS_CAST(Object);
	CLASS_CAST(IWinProc);
	CLASS_CAST(AbilityListWinProc);
	return nullptr;
}

// This method returns a combinations of values in UTFWin::EventFlags.
// The combination determines what types of events (messages) this window procedure receives.
// By default, it receives mouse/keyboard input and advanced messages.
int AbilityListWinProc::GetEventFlags() const
{
	return kEventFlagBasicInput | kEventFlagAdvanced;
}

// The method that receives the message. The first thing you should do is probably
// checking what kind of message was sent...
bool AbilityListWinProc::HandleUIMessage(IWindow* window, const Message& message)
{
	
	// Return true if the message was handled, and therefore no other window procedure should receive it.
	return false;
}
