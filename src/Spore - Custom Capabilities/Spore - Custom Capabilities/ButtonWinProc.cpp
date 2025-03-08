#include "stdafx.h"
#include "ButtonWinProc.h"
#include "cCustomAbilityManager.h"
//#include "HologramScoutMod.h"

ButtonWinProc::ButtonWinProc(UTFWin::IWindow* rootWindow, uint32_t attkID)
{
	this->mpWindowOwner = rootWindow;
	SetAbility(attkID);
}


ButtonWinProc::~ButtonWinProc()
{
}

// For internal use, do not modify.
int ButtonWinProc::AddRef()
{
	return DefaultRefCounted::AddRef();
}

// For internal use, do not modify.
int ButtonWinProc::Release()
{
	return DefaultRefCounted::Release();
}

// You can extend this function to return any other types your class implements.
void* ButtonWinProc::Cast(uint32_t type) const
{
	CLASS_CAST(Object);
	CLASS_CAST(IWinProc);
	CLASS_CAST(ButtonWinProc);
	return nullptr;
}

// This method returns a combinations of values in UTFWin::EventFlags.
// The combination determines what types of events (messages) this window procedure receives.
// By default, it receives mouse/keyboard input and advanced messages.
int ButtonWinProc::GetEventFlags() const
{
	return kEventFlagBasicInput | kEventFlagAdvanced | kEventFlagUpdate;
}

// The method that receives the message. The first thing you should do is probably
// checking what kind of message was sent...
bool ButtonWinProc::HandleUIMessage(IWindow* window, const Message& message)
{
	if (!GameNounManager.GetAvatar())
	{
		return false;
	}
	auto avatar = GameNounManager.GetAvatar();
	auto ability = avatar->GetAbility(mAbilityID);

	if (message.IsType(MessageType::kMsgButtonSelect))
	{
		CustomAbilityManager.TriggerSkill(ability, avatar);
		return true;
	}


	if (message.IsType(MessageType::kMsgMouseDown))
	{
		if (message.Mouse.IsRightButton())
		{
			SetAbility(id("scan"));
			return true;
		}
	}

	if (message.IsType(MessageType::kMsgUpdate))
	{
		auto button = object_cast<UTFWin::IButton>(window);
		string str;
		
		//char flags[32]{" "};
		//auto thing = _itoa_s(button->GetButtonStateFlags(), flags, 2);
		//SporeDebugPrint(flags);
		
		if (CustomAbilityManager.CanUseAbility(ability, avatar))
		{
			button->SetButtonStateFlag(4, false);
		}
		else
		{
			button->SetButtonStateFlag(4, true);
		}

		auto val = 255 * !(button->GetButtonStateFlags() & 4);
		if (window->FindWindowByID(0x0))
			window->FindWindowByID(0x0)->SetShadeColor(Math::Color(val, val, val, 240));
	}


	// Return true if the message was handled, and therefore no other window procedure should receive it.
	return false;
}

void ButtonWinProc::SetAbility(uint32_t abilityID)
{
	vector<IWindow*> windowsToRemove;
	for (UTFWin::IWindow* window : mpWindowOwner->children())
	{
		if (window != mpWindowOwner && window)
		{
			windowsToRemove.push_back(window);
		}
	}

	for (UTFWin::IWindow* window : windowsToRemove)
	{
		mpWindowOwner->RemoveWindow(window);
	}

	UTFWin::IWinProc* winProc = mpWindowOwner->GetNextWinProc();
	while (winProc != nullptr)
	{
		if (winProc->TYPE == UTFWin::Tooltip::TYPE)
		{
			mpWindowOwner->RemoveWinProc(winProc);
		}
		winProc = mpWindowOwner->GetNextWinProc(winProc);
	}

	auto window = new UTFWin::Window();

	window->SetControlID(0x0);
	mpWindowOwner->AddWindow(window);

	auto skill = CustomAbilityManager.GetAbility(abilityID);
	if (!skill)
	{
		mAbilityID = -1;
		return;
	}
	ResourceKey imgKey = ResourceKey(skill->mVerbIconImageID, TypeIDs::png, id("CustomAbilityIcons"));

	UTFWin::ImageDrawable* imageDrawable = new UTFWin::ImageDrawable();
	window->SetDrawable(imageDrawable);
	window->SetShadeColor(Math::Color(255, 255, 255, 240));

	window->SetArea(Math::Rectangle(8, 8, 28, 28));
	UTFWin::ImageDrawable::SetImageForWindow(window, imgKey);

	window->SetVisible(true);
	window->SetFlag(UTFWin::WindowFlags::kWinFlagIgnoreMouse, true);
	//creatureAbilityRolloverValueDescription

	Math::Vector2 minLevel;
	string16 tooltip = skill->nName.GetText();
	if (App::Property::GetVector2(skill->mpPropList.get(), 0x04052A86, minLevel))
	{
		if (minLevel.x != 0)
		{
			tooltip += string16(u"").sprintf(u" - Level %i", int(minLevel.x));
		}
	}
	mpWindowOwner->AddWinProc(UTFWin::CreateTooltip(tooltip.c_str()));

	mAbilityID = abilityID;
}
