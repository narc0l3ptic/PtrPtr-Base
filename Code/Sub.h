#pragma once
#include "AbstractOption.h"

class Sub
{
public:
	std::string m_Name;
	size_t m_ID;
	int m_CurrentOptionIndex = 0;
	std::vector<AbstractOption*> m_Options;

	Sub(std::string name, size_t id)
		: m_Name{ name }, m_ID{ id } {}
	~Sub() {}

	void add_option(AbstractOption &option)
	{
		m_Options.push_back(&option);
	}
};