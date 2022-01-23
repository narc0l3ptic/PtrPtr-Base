#pragma once
#include "UIManagement.h"
#include "AbstractOption.h"

class SubOption : public AbstractOption
{
	int m_ParentSubIndex{};
	int m_ChildSubIndex{};
	std::function<void()> m_Function{};
public:
	SubOption(std::string name, int parentSubIndex, int childSubIndex, std::function<void()> tempFunction = []{})
		: AbstractOption{ name, OptionType::SubOption }, m_ParentSubIndex{ parentSubIndex }, m_ChildSubIndex{ childSubIndex }, m_Function{ tempFunction } {}
	~SubOption() {}

	virtual void do_action() override
	{
		g_UIManagement->m_ChildParentSubs[m_ParentSubIndex].push_to_sub_stack(m_ChildSubIndex);
		m_Function();
	}
};