#pragma once
#include "AbstractOption.h"

class BoolOption : public AbstractOption
{
	bool* m_BoolPtr;
public:
	BoolOption(std::string name, bool& boolToPoint)
		: AbstractOption{ name, OptionType::BoolOption }, m_BoolPtr{ &boolToPoint } {}
	~BoolOption() {}

	virtual void do_action() override
	{
		if (*m_BoolPtr)
		{
			*m_BoolPtr = false;
		}
		else 
		{ 
			*m_BoolPtr = true;
		}
	}

	virtual bool get_value() override
	{
		return *m_BoolPtr;
	}
};