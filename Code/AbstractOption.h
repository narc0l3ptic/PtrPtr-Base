#pragma once
#include "Common.hpp"
#include "Natives.hpp"

using namespace Big;

class AbstractOption
{
public:
	std::string m_Name;
	std::string m_Description;
	OptionType m_Type;

	AbstractOption(std::string tempName, OptionType tempType, std::string tempDescription)
		: m_Name{ tempName }, m_Type{ tempType }, m_Description{ tempDescription } {}
	~AbstractOption() {}

	virtual void do_action() {}
	virtual bool get_value() { return false; } // For bool
};