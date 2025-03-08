#include "stdafx.h"
#include "cCustomAbilityManager.h"

class cCustomAbilityFactory : public App::ISPClassFactory
{
public:
    cCustomAbilityManagerPtr sInstance;

    size_t GetTypes(uint32_t* pDstTypes, size_t count) const override {
        if (count > 0) {
            pDstTypes[0] = cCustomAbilityManager::TYPE;
        }
        return 1;
    }

    Object* Create(uint32_t type, ICoreAllocator* pAllocator) override {
        // Only create the object once, always return the same instance
        if (!sInstance) {
            sInstance = new cCustomAbilityManager();
        }
        return sInstance.get();
    }

    const char* GetName(uint32_t type) const override {
        return "cCustomAbilityFactory";
    }
};