#include "stdafx.h"
#include "ButtonWinProc.h"
#include "cCustomAbilityManager.h"
#include "AbilityListWinProc.h"
//#include "HologramScoutMod.h"

ButtonWinProc::ButtonWinProc(UTFWin::IWindow* rootWindow, uint32_t attkID)
{
	auto winProcs = vector<IWinProc*>();
	auto winProc = rootWindow->GetNextWinProc();
	while (winProc != nullptr)
	{
		if (winProc != this)
		{
			winProcs.push_back(winProc);
			//rootWindow->RemoveWinProc(winProc);
		}
		winProc = rootWindow->GetNextWinProc(winProc);
	}
	for (auto a : winProcs)
	{
		rootWindow->RemoveWinProc(winProc);
	}
	this->mpWindowOwner = rootWindow;
	mpDrawable = mpWindowOwner->GetDrawable();
	mpWindowOwner->SetVisible(true);
	SetAbility(0x0);
	
	
	//rootWindow->SetVisible(false);

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
		if (mAbilityID == 0x0)
		{
			return true;
		}
		CustomAbilityManager.TriggerSkill(ability, avatar);
		return true;
	}


	if (message.IsType(MessageType::kMsgMouseDown))
	{
		if (message.Mouse.IsRightButton())
		{
			ListAbilities();
			//SetAbility(id("scan"));
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
	if (abilityID == 0x0)
	{


		mpWindowOwner->SetDrawable(nullptr);
			// SetShadeColor(colour);

		mpWindowOwner->Revalidate();
		mAbilityID = 0x0;
		return;
	}
	
	mpWindowOwner->SetDrawable(mpDrawable.get());

	mpWindowOwner->SetVisible(true);
	
	/*vector<IWindow*> windowsToRemove;
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
	}*/

	UTFWin::IWinProc* winProc = mpWindowOwner->GetNextWinProc();
	while (winProc != nullptr)
	{
		if (winProc->TYPE == UTFWin::Tooltip::TYPE)
		{
			mpWindowOwner->RemoveWinProc(winProc);
		}
		winProc = mpWindowOwner->GetNextWinProc(winProc);
	}

	//auto window = new UTFWin::Window();

	//window->SetControlID(0x0);
	//mpWindowOwner->AddWindow(window);

	auto skill = CustomAbilityManager.GetAbility(abilityID);
	if (!skill)
	{
		mAbilityID = -1;
		return;
	}
	ResourceKey imgKey = ResourceKey(skill->mVerbIconImageID, TypeIDs::png, id("CustomAbilityIcons"));
	ImagePtr img;
	UTFWin::Image::GetImage(imgKey, img);
	((SporeStdDrawable*)mpWindowOwner->GetDrawable())->SetIconImage(img.get(), 0);
	/*((SporeStdDrawable*)mpWindowOwner->GetDrawable())->SetIconImage(img.get(), 1);
	((SporeStdDrawable*)mpWindowOwner->GetDrawable())->SetIconImage(img.get(), 2);
	((SporeStdDrawable*)mpWindowOwner->GetDrawable())->SetIconImage(img.get(), 3);
	((SporeStdDrawable*)mpWindowOwner->GetDrawable())->SetIconImage(img.get(), 4);
	((SporeStdDrawable*)mpWindowOwner->GetDrawable())->SetIconImage(img.get(), 5);
	((SporeStdDrawable*)mpWindowOwner->GetDrawable())->SetIconImage(img.get(), 6);
	((SporeStdDrawable*)mpWindowOwner->GetDrawable())->SetIconImage(img.get(), 7);*/

	mpWindowOwner->Revalidate();
	
	//UTFWin::ImageDrawable* imageDrawable = new UTFWin::ImageDrawable();
	//window->SetDrawable(imageDrawable);
	//window->SetShadeColor(Math::Color(255, 255, 255, 240));

	//window->SetArea(Math::Rectangle(8, 8, 28, 28));
	//UTFWin::ImageDrawable::SetImageForWindow(window, imgKey);

	//window->SetVisible(true);
	//window->SetFlag(UTFWin::WindowFlags::kWinFlagIgnoreMouse, true);
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

	AbilityListWinProc::sbRemove = true;
}






void ButtonWinProc::ListAbilities()
{
	// Init. variables.
	auto mainWindow = WindowManager.GetMainWindow();
	bool isBelow = false;
	bool isLeft = false;
	bool isSpaceGame = Simulator::IsSpaceGame();
	auto avatar = GameNounManager.GetAvatar();
	const auto abilities = avatar->mpSpeciesProfile->mAbilities;

	auto parent = mpWindowOwner->GetParent();

	// Get abs. position, and size of main window.
	auto position = mpWindowOwner->GetRealArea();
	auto mainSize = mainWindow->GetRealArea();

	Math::Point topLeft = parent->ToGlobalCoordinates(Math::Point(position.x1, position.y1));
	Math::Point bottomRight = parent->ToGlobalCoordinates(Math::Point(position.x2, position.y2));

	SporeDebugPrint("%f, %f, %f, %f", topLeft.y, bottomRight.y, mainSize.y1, mainSize.y2);
	if (topLeft.y > mainSize.y2 - bottomRight.y)
	{
		isBelow = true;
	}

	if (topLeft.x > mainSize.x2 - bottomRight.x)
	{
		isLeft = true;
	}


	// get all abilities that are valid to switch to
	vector<int> abilityIndicies = vector<int>();
	for (int i = 0; i < abilities.size(); i++)
	{
		const auto& ability = abilities[i];
		bool isValid = false;
		bool onlySpaceStage = false;
		if (App::Property::GetBool(ability->mpPropList.get(), id("allowAbilitySelection"), isValid))
		{
			if (isValid == false)
			{
				continue;
			}
		}
		if (App::Property::GetBool(ability->mpPropList.get(), id("spaceStageOnly"), onlySpaceStage))
		{
			if (onlySpaceStage && !isSpaceGame)
			{
				continue;
			}
		}

		abilityIndicies.push_back(i);

	}

	// Start to create the actual ability options
	int yInterval = 24;
	int xInterval = 161;

	int acceptibleHeight = mainSize.y2 / 3;
	uint32_t verticalNumber = max(6, acceptibleHeight / yInterval);

	// remember: negative values is up
	if (isBelow)
	{
		yInterval = -yInterval;
	}
	if (!isLeft)
	{
		xInterval *= -1;
	}

	for (int i = 0; i < abilityIndicies.size(); i++)
	{
		UILayout* layout = new UILayout();
		layout->LoadByID(id("CustomDropdown"));
		
		int ypos = (i % verticalNumber) * yInterval;
		int xpos = floor(i / verticalNumber) * xInterval;


		layout->SetParentWindow(WindowManager.GetMainWindow());//mpWindowOwner.get());

		Math::Point newTopLeft = mpWindowOwner->ToGlobalCoordinates(Math::Point(xpos, ypos));
		Math::Rectangle newArea = Math::Rectangle(newTopLeft.x, newTopLeft.y, newTopLeft.x + 161, newTopLeft.y + 24);

		auto win = layout->FindWindowByID(0x0AAA0061);
		win->SetArea(newArea);
		win->AddWinProc(new AbilityListWinProc(abilityIndicies[i], win, this));
		win->SetShadeColor(0xFFFFFFFF);
		win->SetFlag(UTFWin::WindowFlags::kWinFlagAlwaysInFront, true);
		mpWindowOwner->SetFlag(WindowFlags::kWinFlagAlwaysInFront, true);
		mpWindowOwner->Revalidate();
		win->Revalidate();

		layout->SetVisible(true);
	}
	
}
