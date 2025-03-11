#pragma once

#include <memory>
#include <cstdint>

enum ModuleName
{
	NETWORK,
	STORAGE,
	USER_INTERFACE
};

class ObserverableModule
{
public:
	explicit ObserverableModule();
	virtual ~ObserverableModule();

	virtual void link(const ObserverableModule* module, const ModuleName module_name) = 0;
	virtual void publishEvent(uint32_t stataus) = 0;
};