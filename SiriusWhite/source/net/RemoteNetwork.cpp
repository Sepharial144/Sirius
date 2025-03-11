#include "RemoteNetwork.hpp"
#include "JsonProtocol.hpp"

RemoteNetwork::RemoteNetwork() :
	m_protocol{ std::make_unique<JsonProtocol>() }
{
}

RemoteNetwork::~RemoteNetwork()
{
}

void RemoteNetwork::fetch(const char* some_net_api)
{
	m_protocol->translate(0);
}

void RemoteNetwork::send(const char* some_net_send_api)
{
	m_protocol->translate(1);
}

void RemoteNetwork::link(const ObserverableModule* module, const ModuleName module_name)
{
    if (module_name == ModuleName::STORAGE)
        m_pStorage = module;
    if (module_name == ModuleName::USER_INTERFACE)
        m_pUi = module;
}


void RemoteNetwork::publishEvent(uint32_t stataus)
{

}