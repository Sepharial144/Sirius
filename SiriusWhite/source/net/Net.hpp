#pragma once 

#include "logger/logger.hpp"

class Net
{
public:
	explicit Net();
	virtual ~Net();
	
	virtual void fetch(const char* some_net_api) = 0;
	virtual void send(const char* some_net_send_api) = 0;
private:

};