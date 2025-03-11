#pragma once 

#include <cstdint>

enum ProtocolStatus {
	NO_STATUS,
	SEND,
	REQUEST
};

class Protocol
{
public:
	explicit Protocol();
	virtual ~Protocol();

	virtual void translate(int32_t status) = 0;
};