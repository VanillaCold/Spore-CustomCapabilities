#include "stdafx.h"
#include "ShowUICheat.h"

ShowUICheat::ShowUICheat()
{
}


ShowUICheat::~ShowUICheat()
{
}


void ShowUICheat::ParseLine(const ArgScript::Line& line)
{
	// This method is called when your cheat is invoked.
	// Put your cheat code here.
	CustomAbilityManager.CreateAbilityUI();
}

const char* ShowUICheat::GetDescription(ArgScript::DescriptionMode mode) const
{
	if (mode == ArgScript::DescriptionMode::Basic) {
		return "This cheat does something.";
	}
	else {
		return "ShowUICheat: Elaborate description of what this cheat does.";
	}
}
