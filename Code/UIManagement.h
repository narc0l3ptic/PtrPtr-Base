#pragma once
#include "ParentSub.h"
#include "Natives.hpp"
#include "Timer.hpp"

using namespace Big;

enum class Font : std::int32_t
{
	ChaletLondon = 0,
	HouseScript = 1,
	Monospace = 2,
	Wingdings = 3,
	ChaletComprimeCologne = 4,
	Pricedown = 7
};

class UIManagement
{
	float width = 0.17;
	float parent_bar_height = 0.03;
	float option_height = 0.03;
	float y_pos = 0.1f;
	float x_pos = 0.7f;
	float left_text_padding = 0.005;
	float right_sub_padding = 0.05;
	
	float parent_sub_text_size = 0.45;
	float option_text_size = 0.35;

	int max_options_displayed = 12;

	Color text_colour = Color{ 0, 0, 0, 255 };
	Color text_colour_selected = Color{ 255, 255, 255, 255 };
	Color background_colour = Color{ 255, 255, 255, 255 };
	Color background_colour_selected = Color{ 0, 0, 0, 255 };
public:
	int m_SelectedParentSub{ 0 };
	std::vector<ParentSub> m_ChildParentSubs{};

	int add_parent_sub(std::string name)
	{
		int tempSize = m_ChildParentSubs.size();
		m_ChildParentSubs.push_back(ParentSub{ name, m_ChildParentSubs.size() });
		return tempSize;
	}

	void init()
	{
	}

	void check_and_handle_input()
	{
		bool LeftParentPressed = IsKeyPressed(VK_NUMPAD7);
		bool RightParentPressed = IsKeyPressed(VK_NUMPAD9);
		bool UpPressed = IsKeyPressed(VK_NUMPAD8);
		bool DownPressed = IsKeyPressed(VK_NUMPAD2);
		bool EnterPressed = IsKeyPressed(VK_NUMPAD5);
		bool BackPressed = IsKeyPressed(VK_NUMPAD0);

		Sub* current_sub = &m_ChildParentSubs[m_SelectedParentSub].m_ChildSubs[m_ChildParentSubs[m_SelectedParentSub].get_top_sub_stack_index()];
		int* current_option_index = &m_ChildParentSubs[m_SelectedParentSub].m_ChildSubs[m_ChildParentSubs[m_SelectedParentSub].get_top_sub_stack_index()].m_CurrentOptionIndex;

		static Timer parentTimer(0ms);
		parentTimer.SetDelay(std::chrono::milliseconds(150ms));

		static Timer boolTimer(0ms);
		boolTimer.SetDelay(std::chrono::milliseconds(500ms));
		if (LeftParentPressed && parentTimer.Update())
		{
			if (m_SelectedParentSub == 0)
			{
				m_SelectedParentSub = m_ChildParentSubs.size() - 1;
			}
			else
			{
				m_SelectedParentSub--;
			}
		}
		if (RightParentPressed && parentTimer.Update())
		{
			if (m_SelectedParentSub == m_ChildParentSubs.size() - 1)
			{
				m_SelectedParentSub = 0;
			}
			else
			{
				m_SelectedParentSub++;
			}
		}

		static Timer optionTimer(0ms);
		optionTimer.SetDelay(std::chrono::milliseconds(150ms));
		if (UpPressed && optionTimer.Update())
		{
			if (*current_option_index == 0)
			{
				*current_option_index = current_sub->m_Options.size() - 1;
			}
			else
			{
				(*current_option_index)--;
			}
		}
		if (DownPressed && optionTimer.Update())
		{
			if (*current_option_index == current_sub->m_Options.size() - 1)
			{
				*current_option_index = 0;
			}
			else
			{
				(*current_option_index)++;
			}
		}
		if (EnterPressed && parentTimer.Update())
		{
			if (m_ChildParentSubs[m_SelectedParentSub].m_ChildSubs[m_ChildParentSubs[m_SelectedParentSub].get_top_sub_stack_index()].m_Options.size() > 0)
			{
				current_sub->m_Options[current_sub->m_CurrentOptionIndex]->do_action();
			}
		}
		if (BackPressed && parentTimer.Update())
		{
			if (m_ChildParentSubs[m_SelectedParentSub].m_SubStackIndex.size() > 1)
			{
				m_ChildParentSubs[m_SelectedParentSub].m_SubStackIndex.pop_back();
			}
		}
	}

	void check_and_handle_input_2()
	{
		bool OpenClosePressed = IsKeyPressed(0x50);
		static Timer openCloseTimer(0ms);
		openCloseTimer.SetDelay(std::chrono::milliseconds(100ms));
		if (OpenClosePressed && openCloseTimer.Update())
		{
			g_Opened = g_Opened ? false : true;
		}
	}

	void tick()
	{
		if (g_Opened)
		{
			Sub* current_sub = &m_ChildParentSubs[m_SelectedParentSub].m_ChildSubs[m_ChildParentSubs[m_SelectedParentSub].get_top_sub_stack_index()];

			check_and_handle_input();
			check_and_handle_input_2();

			// Draw parent bar.
			{
				size_t tempParentWidths = width / m_ChildParentSubs.size(); // Getting the allocated sizes for each parent sub.
				for (int i{}; i < m_ChildParentSubs.size(); ++i) // For each parent sub.
				{
					if (i == m_SelectedParentSub) // If we are currently iterating on the selected parent sub do the contained.
					{
						GRAPHICS::DRAW_RECT(x_pos + (i * (width / m_ChildParentSubs.size())), y_pos, (width / m_ChildParentSubs.size()), parent_bar_height, text_colour_selected.r, text_colour_selected.g, text_colour_selected.b, text_colour_selected.a, 1);
						draw_centered_text(m_ChildParentSubs[i].m_Name.c_str(), // I split this function call up along the lines because it was too long :(
							x_pos + (i * (width / m_ChildParentSubs.size())),
							y_pos - (get_text_height(Font::ChaletLondon, parent_sub_text_size) / 1.5f),
							parent_sub_text_size,
							Font::ChaletLondon,
							background_colour_selected,
							false, false);
					}
					else
					{
						GRAPHICS::DRAW_RECT(x_pos + (i * (width / m_ChildParentSubs.size())), y_pos, (width / m_ChildParentSubs.size()), parent_bar_height, text_colour.r, text_colour.g, text_colour.b, text_colour.a, 1);
						draw_centered_text(m_ChildParentSubs[i].m_Name.c_str(), // I split this function call up along the lines because it was too long :(
							x_pos + (i * (width / m_ChildParentSubs.size())),
							y_pos - (get_text_height(Font::ChaletLondon, parent_sub_text_size) / 1.5f),
							parent_sub_text_size,
							Font::ChaletLondon,
							background_colour,
							false, false);
					}
				}
			}

			// Draw options.
			{
				if (m_ChildParentSubs[m_SelectedParentSub].m_ChildSubs.size() > 0)
				{
					int start = 0;
					int end = current_sub->m_Options.size() > max_options_displayed ? max_options_displayed : current_sub->m_Options.size();
					if (current_sub->m_Options.size() > max_options_displayed && current_sub->m_CurrentOptionIndex >= max_options_displayed)
					{
						start = current_sub->m_CurrentOptionIndex - max_options_displayed + 1;
						end = current_sub->m_CurrentOptionIndex + 1;
					}

					for (int i = start, j = 0; i < /*current_sub->m_Options.size()*/end; ++i, ++j)
					{
						if (i == current_sub->m_CurrentOptionIndex)
						{
							GRAPHICS::DRAW_RECT((x_pos - (width / m_ChildParentSubs.size()) / 2) + width / 2, y_pos + ((/*i*/j +1) * option_height) - (option_height - parent_bar_height), width, option_height, text_colour_selected.r, text_colour_selected.g, text_colour_selected.b, text_colour_selected.a, 1); // This is very confusing so dont bother reading it.

							if (current_sub->m_Options[i]->m_Type == OptionType::RegularOption)
							{
								draw_left_text(current_sub->m_Options[i]->m_Name.c_str(),
									x_pos - (width / m_ChildParentSubs.size()) / 2 + left_text_padding,
									y_pos + parent_bar_height / 2 + (/*i*/j * option_height),
									option_text_size,
									Font::ChaletLondon,
									background_colour_selected,
									false, false);
							}
							if (current_sub->m_Options[i]->m_Type == OptionType::SubOption)
							{
								draw_left_text(current_sub->m_Options[i]->m_Name.c_str(),
									x_pos - (width / m_ChildParentSubs.size()) / 2 + left_text_padding,
									y_pos + parent_bar_height / 2 + (/*i*/j * option_height),
									option_text_size,
									Font::ChaletLondon,
									background_colour_selected,
									false, false);

								draw_right_text(">",
									x_pos + width - right_sub_padding,
									y_pos + parent_bar_height / 2 + (/*i*/j * option_height),
									option_text_size,
									Font::ChaletLondon,
									background_colour_selected,
									false, false);
							}
							if (current_sub->m_Options[i]->m_Type == OptionType::BoolOption)
							{
								draw_left_text(current_sub->m_Options[i]->m_Name.c_str(),
									x_pos - (width / m_ChildParentSubs.size()) / 2 + left_text_padding,
									y_pos + parent_bar_height / 2 + (/*i*/j * option_height),
									option_text_size,
									Font::ChaletLondon,
									background_colour_selected,
									false, false);

								if (current_sub->m_Options[i]->get_value() == true)
								{
									draw_right_text("ON",
										x_pos + width - right_sub_padding,
										y_pos + parent_bar_height / 2 + (/*i*/j * option_height),
										option_text_size,
										Font::ChaletLondon,
										Color{ 0, 255, 0, 255 },
										false, false);
								}
								else
								{
									draw_right_text("OFF",
										x_pos + width - right_sub_padding,
										y_pos + parent_bar_height / 2 + (/*i*/j * option_height),
										option_text_size,
										Font::ChaletLondon,
										Color{ 255, 0, 0, 255 },
										false, false);
								}
							}
						}
						else
						{
							GRAPHICS::DRAW_RECT((x_pos - (width / m_ChildParentSubs.size()) / 2) + width / 2, y_pos + ((j + 1) * option_height) - (option_height - parent_bar_height), width, option_height, text_colour.r, text_colour.g, text_colour.b, text_colour.a, 1); // This is very confusing so dont bother reading it.

							if (current_sub->m_Options[i]->m_Type == OptionType::RegularOption)
							{
								draw_left_text(current_sub->m_Options[i]->m_Name.c_str(),
									x_pos - (width / m_ChildParentSubs.size()) / 2 + left_text_padding,
									y_pos + parent_bar_height / 2 + (/*i*/j * option_height),
									option_text_size,
									Font::ChaletLondon,
									background_colour,
									false, false);
							}
							if (current_sub->m_Options[i]->m_Type == OptionType::SubOption)
							{
								draw_left_text(current_sub->m_Options[i]->m_Name.c_str(),
									x_pos - (width / m_ChildParentSubs.size()) / 2 + left_text_padding,
									y_pos + parent_bar_height / 2 + (/*i*/j * option_height),
									option_text_size,
									Font::ChaletLondon,
									background_colour,
									false, false);

								draw_right_text(">",
									x_pos + width - right_sub_padding,
									y_pos + parent_bar_height / 2 + (/*i*/j * option_height),
									option_text_size,
									Font::ChaletLondon,
									background_colour,
									false, false);
							}
							if (current_sub->m_Options[i]->m_Type == OptionType::BoolOption)
							{
								draw_left_text(current_sub->m_Options[i]->m_Name.c_str(),
									x_pos - (width / m_ChildParentSubs.size()) / 2 + left_text_padding,
									y_pos + parent_bar_height / 2 + (/*i*/j * option_height),
									option_text_size,
									Font::ChaletLondon,
									background_colour,
									false, false);

								if (current_sub->m_Options[i]->get_value() == true)
								{
									draw_right_text("ON",
										x_pos + width - right_sub_padding,
										y_pos + parent_bar_height / 2 + (/*i*/j * option_height),
										option_text_size,
										Font::ChaletLondon,
										Color{ 0, 255, 0, 255 },
										false, false);
								}
								else
								{
									draw_right_text("OFF",
										x_pos + width - right_sub_padding,
										y_pos + parent_bar_height / 2 + (/*i*/j * option_height),
										option_text_size,
										Font::ChaletLondon,
										Color{ 255, 0, 0, 255 },
										false, false);
								}
							}
						}
					}
				}
			}
		}
	}

	void draw_left_text(const char* text, float x, float y, float size, Font font, Color colour, bool outline, bool shadow)
	{
		HUD::SET_TEXT_SCALE(size, size);
		HUD::SET_TEXT_FONT(static_cast<int>(font));
		HUD::SET_TEXT_COLOUR(colour.r, colour.g, colour.b, colour.a);
		if (outline)
			HUD::SET_TEXT_OUTLINE();
		if (shadow)
			HUD::SET_TEXT_DROP_SHADOW();
		
		HUD::BEGIN_TEXT_COMMAND_DISPLAY_TEXT("STRING");
		HUD::ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME(text);
		HUD::END_TEXT_COMMAND_DISPLAY_TEXT(x, y, 0);
	}

	void draw_right_text(const char* text, float x, float y, float size, Font font, Color colour, bool outline, bool shadow)
	{
		HUD::SET_TEXT_WRAP(0.f, x);
		HUD::SET_TEXT_RIGHT_JUSTIFY(true);
		draw_left_text(text, x, y, size, font, colour, outline, shadow);
	}

	void draw_centered_text(const char* text, float x, float y, float size, Font font, Color colour, bool outline, bool shadow)
	{
		HUD::SET_TEXT_CENTRE(true);
		draw_left_text(text, x, y, size, font, colour, outline, shadow);
	}

	float get_text_height(Font font, float size)
	{
		return HUD::_GET_TEXT_SCALE_HEIGHT(size, static_cast<int>(font));
	}
};

inline std::unique_ptr<UIManagement> g_UIManagement;