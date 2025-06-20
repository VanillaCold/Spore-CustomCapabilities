#include "stdafx.h"
#include "AbilityListWinProc.h"

AbilityListWinProc::AbilityListWinProc(int abilityIndex, IWindow* window, ButtonWinProc* btnProc)
{
	sbRemove = false;
	this->abilityIndex = abilityIndex;
	mpWindowOwner = window;
	mpOwnerProc = btnProc;

	auto skill = GameNounManager.GetAvatar()->GetAbility(abilityIndex);

	ResourceKey imgKey = ResourceKey(skill->mVerbIconImageID, TypeIDs::png, id("CustomAbilityIcons"));
	auto win = window->FindWindowByID(0xCDFB1002);

	if (win)
	{
		UTFWin::ImageDrawable* imageDrawable = new UTFWin::ImageDrawable();
		win->SetDrawable(imageDrawable);
		win->SetShadeColor(Math::Color(0, 0, 0, 240));

		UTFWin::ImageDrawable::SetImageForWindow(win, imgKey);
	}

	auto capt = window->FindWindowByID(id("CustomText"));
	if (capt)
	{
		capt->SetCaption(skill->nName.GetText());
	}
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
	return kEventFlagBasicInput | kEventFlagAdvanced | kEventFlagUpdate;
}

// The method that receives the message. The first thing you should do is probably
// checking what kind of message was sent...
bool AbilityListWinProc::HandleUIMessage(IWindow* window, const Message& message)
{
	if (message.IsType(UTFWin::MessageType::kMsgButtonClick))
	{
		mpOwnerProc->SetAbility(GameNounManager.GetAvatar()->GetAbility(abilityIndex)->mpPropList->GetResourceKey().instanceID);
	}
	if (message.IsType(UTFWin::MessageType::kMsgUpdate))
	{
		if (sbRemove)
		{
			mpWindowOwner->GetParent()->RemoveWindow(mpWindowOwner.get());
		}
	}
	// Return true if the message was handled, and therefore no other window procedure should receive it.
	return false;
}

bool AbilityListWinProc::sbRemove = false;