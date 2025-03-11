#pragma once 

#include "Net.hpp"
#include "Protocol.hpp"
#include "logger/logger.hpp"
#include "ObserverableModule.hpp"

#include <memory>


class LocalNetwork: public Net, public ObserverableModule
{
public:
	explicit LocalNetwork();
	virtual ~LocalNetwork();
	
	void fetch(const char* some_net_api) override;
	void send(const char* some_net_send_api) override;

	void link(const ObserverableModule* module, const ModuleName module_name) override;
	void publishEvent(uint32_t stataus) override;
private:
	std::unique_ptr<Protocol> m_protocol;
	const ObserverableModule* m_pStorage = nullptr;
	const ObserverableModule* m_pUi = nullptr;
};