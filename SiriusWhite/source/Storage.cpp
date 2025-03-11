#include "Storage.hpp"
#include "net/LocalNetwork.hpp"
#include "net/RemoteNetwork.hpp"
#include "DataTransferObject.hpp"

Storage::Storage()
{
}

Storage::~Storage()
{
}

auto Storage::getData() -> std::shared_ptr<DataTransferObject>
{
	return std::make_shared<DataTransferObject>();
}

auto Storage::reqData() -> std::shared_ptr<DataTransferObject>
{
	return std::make_shared<DataTransferObject>();
}



[[maybe_unused]] void Storage::link(const ObserverableModule* module, const ModuleName module_name)
{
    if (module_name == ModuleName::NETWORK)
        m_pNetwork = module;
    if (module_name == ModuleName::USER_INTERFACE)
        m_pUi = module;
}


[[maybe_unused]] void Storage::publishEvent(uint32_t stataus)
{

}
