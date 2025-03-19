#pragma once

#include <map>
#include <exception>

#include "logger/logger.hpp"


template<class Key, class TargetClass>
class CustomStorage
{
public:
	explicit CustomStorage() {};
	virtual ~CustomStorage() {};

	void insert(const std::pair<Key, TargetClass*> value) 
	{
		m_customStorage.insert(value);
	}

	auto find(const Key key_value) -> std::map<Key, TargetClass*>::iterator
	{
		auto it = std::find_if(m_customStorage.begin(), m_customStorage.end(), [](const Key key) {
			return key == key_value;
		});
		return it;
	}

	void erase(const Key key)
	{
		auto it = find(key);
		if (it != m_customStorage.end())
			m_customStorage.erase(it);
	}

	void erase(const std::map<Key, TargetClass*>::iterator iterator)
	{
		if (iterator != m_customStorage.end())
			m_customStorage.erase(iterator);
	}

private:
	std::map<Key, TargetClass*> m_customStorage;
};