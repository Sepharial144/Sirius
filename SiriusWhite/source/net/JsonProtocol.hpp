#pragma once

#include "Protocol.hpp"

#include <cstdint>

class JsonProtocol: public Protocol
{
public:
	explicit JsonProtocol();
	virtual ~JsonProtocol();

	void translate(int32_t status) override;
};