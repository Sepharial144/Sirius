#include "LocalNetwork.hpp"
#include "JsonProtocol.hpp"

LocalNetwork::LocalNetwork() :
	m_protocol{ std::make_unique<JsonProtocol>() }
{
}

LocalNetwork::~LocalNetwork()
{
}

void LocalNetwork::fetch(const char* some_net_api)
{
	m_protocol->translate(0);
}

void LocalNetwork::send(const char* some_net_send_api)
{
	m_protocol->translate(1);
}

void LocalNetwork::link(const ObserverableModule* module, const ModuleName module_name)
{
    if (module_name == ModuleName::STORAGE)
        m_pStorage = module;
    if (module_name == ModuleName::USER_INTERFACE)
        m_pUi = module;
}


void LocalNetwork::publishEvent(uint32_t stataus)
{

}