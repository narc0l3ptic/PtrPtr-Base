#pragma once
#include "Common.hpp"
#include "Natives.hpp"

using namespace Big;

class AbstractOption
{
public:
	std::string m_Name;
	OptionType m_Type;

	AbstractOption(std::string tempName, OptionType tempType)
		: m_Name{ tempName }, m_Type{ tempType } {}
	~AbstractOption() {}

	virtual void do_action() {}
	virtual bool get_value() { return false; } // For bool
};