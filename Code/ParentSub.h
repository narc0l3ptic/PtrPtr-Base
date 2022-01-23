#pragma once
#include "Sub.h"

class ParentSub
{
public:
	std::string m_Name;
	size_t m_ID;
	std::vector<Sub> m_ChildSubs{};
	std::vector<int> m_SubStackIndex{ 0 };

	ParentSub(std::string name, size_t id)
		: m_Name{ name }, m_ID{ id } {}
	~ParentSub() {}

	int add_child_sub(std::string name)
	{
		int tempIndex = m_ChildSubs.size();
		m_ChildSubs.push_back(Sub{ name, m_ChildSubs.size() });
		return tempIndex;
	}
	void push_to_sub_stack(int index)
	{
		m_SubStackIndex.push_back(index);
	}
	int get_top_sub_stack_index()
	{
		return m_SubStackIndex[m_SubStackIndex.size() - 1];
	}
};