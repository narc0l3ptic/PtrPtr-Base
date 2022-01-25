#pragma once
#include "AbstractOption.h"

class RegularOption : public AbstractOption
{
public:
	std::function<void()> m_Function{};

	RegularOption(std::string name, std::function<void()> tempFunction, std::string tempDescription = "")
		: AbstractOption{ name, OptionType::RegularOption, tempDescription }, m_Function{ tempFunction } {}
	~RegularOption() {}

	virtual void do_action() override
	{
		m_Function();
	}
};