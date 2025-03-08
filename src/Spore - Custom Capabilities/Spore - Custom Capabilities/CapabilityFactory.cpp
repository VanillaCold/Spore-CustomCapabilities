#include "stdafx.h"
#include "cCapabilityManager.h"

class CapabilityFactory : public App::ISPClassFactory
{
public:
    cCapabilityManagerPtr sInstance;

    size_t GetTypes(uint32_t* pDstTypes, size_t count) const override {
        if (count > 0) {
            pDstTypes[0] = cCapabilityManager::TYPE;
        }
        return 1;
    }

    Object* Create(uint32_t type, ICoreAllocator* pAllocator) override {
        // Only create the object once, always return the same instance
        if (!sInstance) {
            sInstance = new cCapabilityManager();
        }
        return sInstance.get();
    }

    const char* GetName(uint32_t type) const override {
        return "cCapabilityFactory";
    }
};