#include "UserInterface.hpp"

UserInterface::UserInterface() 
{
}

UserInterface::~UserInterface() 
{
}

void UserInterface::link(const ObserverableModule* module, const ModuleName module_name) 
{
    if (module_name == ModuleName::NETWORK)
        m_pStorage = module;
    if (module_name == ModuleName::STORAGE)
        m_pNetwork = module;
}


void UserInterface::publishEvent(uint32_t stataus)
{

}