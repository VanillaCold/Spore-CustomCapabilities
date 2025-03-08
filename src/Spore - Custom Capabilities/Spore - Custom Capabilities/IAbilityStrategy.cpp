#include "stdafx.h"
#include "IAbilityStrategy.h"

IAbilityStrategy::IAbilityStrategy()
{
}


IAbilityStrategy::~IAbilityStrategy()
{
}

// For internal use, do not modify.
int IAbilityStrategy::AddRef()
{
	return DefaultRefCounted::AddRef();
}

// For internal use, do not modify.
int IAbilityStrategy::Release()
{
	return DefaultRefCounted::Release();
}

// You can extend this function to return any other types your class implements.
void* IAbilityStrategy::Cast(uint32_t type) const
{
	CLASS_CAST(Object);
	CLASS_CAST(IAbilityStrategy);
	return nullptr;
}

bool IAbilityStrategy::sRequiresTarget;