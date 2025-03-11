#pragma once

#include <memory>
#include <cstdint>

#include "net/Net.hpp"
#include "ObserverableModule.hpp"

class DataTransferObject;

class Storage: public ObserverableModule
{
public:
	explicit Storage();
	virtual ~Storage();

	auto getData() -> std::shared_ptr<DataTransferObject>;
	auto reqData() -> std::shared_ptr<DataTransferObject>;

	void link(const ObserverableModule* module, const ModuleName module_name) override;
	void publishEvent(uint32_t stataus) override;

private:
	const ObserverableModule* m_pNetwork = nullptr;
	const ObserverableModule* m_pUi = nullptr;
};