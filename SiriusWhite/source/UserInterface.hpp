#pragma once

#include <memory>
#include <cstdint>

#include "net/Net.hpp"
#include "ObserverableModule.hpp"

class UserInterface: public ObserverableModule
{
public:
	explicit UserInterface();
	virtual ~UserInterface();

	void link(const ObserverableModule* module, const ModuleName module_name) override;
	void publishEvent(uint32_t stataus) override;
private:
	const ObserverableModule* m_pStorage = nullptr;
	const ObserverableModule* m_pNetwork = nullptr;
};