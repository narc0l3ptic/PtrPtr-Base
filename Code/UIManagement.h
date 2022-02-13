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
	float footer_bar_height = 0.03;
	float footer_distance = 0.0;
	float option_height = 0.03;
	float y_pos = 0.1f;
	float x_pos = 0.7f;
	float left_text_padding = 0.005;
	float right_sub_padding = 0.05;

	// For the suggestion box.
	float desc_text_size = 0.3;
	float desc_box_height = 0.07;
	float desc_box_width = 0.2;
	float y_desc = 0.9;
	float x_desc = 0.5;
	float desc_left_text_padding = 0.002;

	float parent_sub_text_size = 0.45;
	float option_text_size = 0.35;

	int max_options_displayed = 12;

	Color text_color = Color{ 255, 255, 255, 255 };
	Color text_color_selected = Color{ 0, 0, 0, 255 };

	Color background_color = Color{ 0, 0, 0, 255 };
	Color background_color_selected = Color{ 255, 255, 255, 255 };

	Color footer_text_color = Color{ 255, 255, 255, 255 };
	Color footer_color = Color{ 102, 102, 102, 255 };

	Color desc_text_color = Color{ 0, 0, 0, 255 };
	Color desc_box_outline_color = Color{ 0, 0, 0, 255 };
	Color desc_box_color = Color{ 102, 102, 102, 255 };

	bool LeftParentPressed;
	bool RightParentPressed;
	bool UpPressed;
	bool DownPressed;
	bool EnterPressed;
	bool BackPressed;
	bool OpenClose;
public:
	bool show_suggestions = true;
	bool play_sounds = true;

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

	void check_input()
	{
		LeftParentPressed = IsKeyPressed(VK_NUMPAD7);
		RightParentPressed = IsKeyPressed(VK_NUMPAD9);
		UpPressed = IsKeyPressed(VK_NUMPAD8);
		DownPressed = IsKeyPressed(VK_NUMPAD2);
		EnterPressed = IsKeyPressed(VK_NUMPAD5);
		BackPressed = IsKeyPressed(VK_NUMPAD0);
		OpenClose = IsKeyPressed(VK_ADD);

		static Timer openCloseTimer(0ms);
		openCloseTimer.SetDelay(std::chrono::milliseconds(250ms));
		if (OpenClose && openCloseTimer.Update())
		{
			g_Opened = g_Opened ? false : true;
			if (play_sounds)
				AUDIO::PLAY_SOUND_FRONTEND(-1, g_Opened ? "SELECT" : "BACK", "HUD_FRONTEND_DEFAULT_SOUNDSET", false);
		}
	}

	void handle_input()
	{
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

			if (play_sounds)
				AUDIO::PLAY_SOUND_FRONTEND(-1, "NAV_LEFT_RIGHT", "HUD_FRONTEND_DEFAULT_SOUNDSET", false);
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

			if (play_sounds)
				AUDIO::PLAY_SOUND_FRONTEND(-1, "NAV_LEFT_RIGHT", "HUD_FRONTEND_DEFAULT_SOUNDSET", false);
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

			if (play_sounds)
				AUDIO::PLAY_SOUND_FRONTEND(-1, "NAV_UP_DOWN", "HUD_FRONTEND_DEFAULT_SOUNDSET", false);
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

			if (play_sounds)
				AUDIO::PLAY_SOUND_FRONTEND(-1, "NAV_UP_DOWN", "HUD_FRONTEND_DEFAULT_SOUNDSET", false);
		}
		if (EnterPressed && parentTimer.Update())
		{
			if (m_ChildParentSubs[m_SelectedParentSub].m_ChildSubs[m_ChildParentSubs[m_SelectedParentSub].get_top_sub_stack_index()].m_Options.size() > 0)
			{
				current_sub->m_Options[current_sub->m_CurrentOptionIndex]->do_action();
				if (play_sounds)
					AUDIO::PLAY_SOUND_FRONTEND(-1, "SELECT", "HUD_FRONTEND_DEFAULT_SOUNDSET", false);
			}
		}
		if (BackPressed && parentTimer.Update())
		{
			if (m_ChildParentSubs[m_SelectedParentSub].m_SubStackIndex.size() > 1)
			{
				m_ChildParentSubs[m_SelectedParentSub].m_SubStackIndex.pop_back();
				if (play_sounds)
					AUDIO::PLAY_SOUND_FRONTEND(-1, "BACK", "HUD_FRONTEND_DEFAULT_SOUNDSET", false);
			}
		}
	}

	void tick()
	{
		check_input();

		if (g_Opened)
		{
			handle_input();

			Sub* current_sub = &m_ChildParentSubs[m_SelectedParentSub].m_ChildSubs[m_ChildParentSubs[m_SelectedParentSub].get_top_sub_stack_index()];

			// Draw parent bar.
			{
				size_t tempParentWidths = width / m_ChildParentSubs.size(); // Getting the allocated sizes for each parent sub.
				for (int i{}; i < m_ChildParentSubs.size(); ++i) // For each parent sub.
				{
					if (i == m_SelectedParentSub) // If we are currently iterating on the selected parent sub do the contained.
					{
						GRAPHICS::DRAW_RECT(x_pos + (i * (width / m_ChildParentSubs.size())), y_pos, (width / m_ChildParentSubs.size()), parent_bar_height, background_color_selected.r, background_color_selected.g, background_color_selected.b, background_color_selected.a, 1);
						draw_centered_text(m_ChildParentSubs[i].m_Name.c_str(), // I split this function call up along the lines because it was too long :(
							x_pos + (i * (width / m_ChildParentSubs.size())),
							y_pos - (get_text_height(Font::ChaletLondon, parent_sub_text_size) / 1.5f),
							parent_sub_text_size,
							Font::ChaletLondon,
							text_color_selected,
							false, false);
					}
					else
					{
						GRAPHICS::DRAW_RECT(x_pos + (i * (width / m_ChildParentSubs.size())), y_pos, (width / m_ChildParentSubs.size()), parent_bar_height, background_color.r, background_color.g, background_color.b, background_color.a, 1);
						draw_centered_text(m_ChildParentSubs[i].m_Name.c_str(), // I split this function call up along the lines because it was too long :(
							x_pos + (i * (width / m_ChildParentSubs.size())),
							y_pos - (get_text_height(Font::ChaletLondon, parent_sub_text_size) / 1.5f),
							parent_sub_text_size,
							Font::ChaletLondon,
							text_color,
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

					for (int i = start, j = 0; i < /*current_sub->m_Options.size()*/ end; ++i, ++j)
					{

						float rectY = y_pos + ((j + 1) * option_height) - (option_height / 2) + (parent_bar_height / 2);

						float textY = rectY - ((option_text_size / 20.f) * 0.75f);

						if (i == current_sub->m_CurrentOptionIndex)
						{
							GRAPHICS::DRAW_RECT((x_pos - (width / m_ChildParentSubs.size()) / 2) + width / 2, rectY, width, option_height, background_color_selected.r, background_color_selected.g, background_color_selected.b, background_color_selected.a, 1); // This is very confusing so dont bother reading it.

							if (current_sub->m_Options[i]->m_Type == OptionType::RegularOption)
							{
								draw_left_text(current_sub->m_Options[i]->m_Name.c_str(),
									x_pos - (width / m_ChildParentSubs.size()) / 2 + left_text_padding,
									textY,
									option_text_size,
									Font::ChaletLondon,
									text_color_selected,
									false, false);
							}
							if (current_sub->m_Options[i]->m_Type == OptionType::SubOption)
							{
								draw_left_text(current_sub->m_Options[i]->m_Name.c_str(),
									x_pos - (width / m_ChildParentSubs.size()) / 2 + left_text_padding,
									textY,
									option_text_size,
									Font::ChaletLondon,
									text_color_selected,
									false, false);

								draw_right_text(">",
									x_pos + width - right_sub_padding,
									textY,
									option_text_size,
									Font::ChaletLondon,
									text_color_selected,
									false, false);
							}
							if (current_sub->m_Options[i]->m_Type == OptionType::BoolOption)
							{
								draw_left_text(current_sub->m_Options[i]->m_Name.c_str(),
									x_pos - (width / m_ChildParentSubs.size()) / 2 + left_text_padding,
									textY,
									option_text_size,
									Font::ChaletLondon,
									text_color_selected,
									false, false);

								if (current_sub->m_Options[i]->get_value())
								{
									draw_right_text("ON",
										x_pos + width - right_sub_padding,
										textY,
										option_text_size,
										Font::ChaletLondon,
										Color{ 0, 255, 0, text_color_selected.a },
										false, false);
								}
								else
								{
									draw_right_text("OFF",
										x_pos + width - right_sub_padding,
										textY,
										option_text_size,
										Font::ChaletLondon,
										Color{ 255, 0, 0, text_color_selected.a },
										false, false);
								}
							}
						}
						else
						{
							GRAPHICS::DRAW_RECT((x_pos - (width / m_ChildParentSubs.size()) / 2) + width / 2, rectY, width, option_height, background_color.r, background_color.g, background_color.b, background_color.a, 1); // This is very confusing so dont bother reading it.

							if (current_sub->m_Options[i]->m_Type == OptionType::RegularOption)
							{
								draw_left_text(current_sub->m_Options[i]->m_Name.c_str(),
									x_pos - (width / m_ChildParentSubs.size()) / 2 + left_text_padding,
									textY,
									option_text_size,
									Font::ChaletLondon,
									text_color,
									false, false);
							}
							if (current_sub->m_Options[i]->m_Type == OptionType::SubOption)
							{
								draw_left_text(current_sub->m_Options[i]->m_Name.c_str(),
									x_pos - (width / m_ChildParentSubs.size()) / 2 + left_text_padding,
									textY,
									option_text_size,
									Font::ChaletLondon,
									text_color,
									false, false);

								draw_right_text(">",
									x_pos + width - right_sub_padding,
									textY,
									option_text_size,
									Font::ChaletLondon,
									text_color,
									false, false);
							}
							if (current_sub->m_Options[i]->m_Type == OptionType::BoolOption)
							{
								draw_left_text(current_sub->m_Options[i]->m_Name.c_str(),
									x_pos - (width / m_ChildParentSubs.size()) / 2 + left_text_padding,
									textY,
									option_text_size,
									Font::ChaletLondon,
									text_color,
									false, false);

								if (current_sub->m_Options[i]->get_value() == true)
								{
									draw_right_text("ON",
										x_pos + width - right_sub_padding,
										textY,
										option_text_size,
										Font::ChaletLondon,
										Color{ 0, 255, 0, text_color.a },
										false, false);
								}
								else
								{
									draw_right_text("OFF",
										x_pos + width - right_sub_padding,
										textY,
										option_text_size,
										Font::ChaletLondon,
										Color{ 255, 0, 0, text_color_selected.a },
										false, false);
								}
							}
						}
					}
				}
			}

			// Draw footer.
			int maxopts = current_sub->m_Options.size() > max_options_displayed ? max_options_displayed : current_sub->m_Options.size();
			float footerRectY = y_pos + ((maxopts + 1) * option_height) - (option_height / 2) + (parent_bar_height / 2) + footer_distance;
			float footerTextY = footerRectY - ((option_text_size / 20.f) * 0.75f);
			GRAPHICS::DRAW_RECT((x_pos - (width / m_ChildParentSubs.size()) / 2) + width / 2,
				footerRectY,
				width,
				footer_bar_height,
				footer_color.r,
				footer_color.g,
				footer_color.b,
				footer_color.a,
				1);

			std::string toDisplay = current_sub->m_Name;
			toDisplay += ": ";
			toDisplay += std::to_string(current_sub->m_CurrentOptionIndex + 1);
			toDisplay += "/";
			toDisplay += std::to_string(current_sub->m_Options.size());

			draw_left_text(toDisplay.c_str(),
				x_pos - (width / m_ChildParentSubs.size()) / 2 + left_text_padding,
				footerTextY,
				option_text_size,
				Font::ChaletLondon,
				footer_text_color,
				false, false);

			// Draw description.
			if (current_sub->m_Options[current_sub->m_CurrentOptionIndex]->m_Description.size() > 0 && show_suggestions)
			{
				float x_offset = 0.005;
				float y_offset = 0.01;

				GRAPHICS::DRAW_RECT(x_desc, y_desc, desc_box_width, desc_box_height, desc_box_outline_color.r, desc_box_outline_color.g, desc_box_outline_color.b, desc_box_outline_color.a, 1);
				GRAPHICS::DRAW_RECT(x_desc, y_desc, desc_box_width - x_offset, desc_box_height - y_offset, desc_box_color.r, desc_box_color.g, desc_box_color.b, desc_box_color.a, 1);
				draw_left_text(current_sub->m_Options[current_sub->m_CurrentOptionIndex]->m_Description.c_str(),
					x_desc + (x_offset)-(desc_box_width / 2) + desc_left_text_padding,
					y_desc - y_offset - ((desc_text_size / 20.f) * 0.75f),
					desc_text_size,
					Font::ChaletLondon,
					desc_text_color,
					false, false);
			}
		}
	}

	void draw_left_text(const char* text, float x, float y, float size, Font font, Color color, bool outline, bool shadow)
	{
		HUD::SET_TEXT_SCALE(size, size);
		HUD::SET_TEXT_FONT(static_cast<int>(font));
		HUD::SET_TEXT_COLOUR(color.r, color.g, color.b, color.a);
		if (outline)
			HUD::SET_TEXT_OUTLINE();
		if (shadow)
			HUD::SET_TEXT_DROP_SHADOW();

		HUD::BEGIN_TEXT_COMMAND_DISPLAY_TEXT("STRING");
		HUD::ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME(text);
		HUD::END_TEXT_COMMAND_DISPLAY_TEXT(x, y, 0);
	}

	void draw_right_text(const char* text, float x, float y, float size, Font font, Color color, bool outline, bool shadow)
	{
		HUD::SET_TEXT_WRAP(0.f, x);
		HUD::SET_TEXT_RIGHT_JUSTIFY(true);
		draw_left_text(text, x, y, size, font, color, outline, shadow);
	}

	void draw_centered_text(const char* text, float x, float y, float size, Font font, Color color, bool outline, bool shadow)
	{
		HUD::SET_TEXT_CENTRE(true);
		draw_left_text(text, x, y, size, font, color, outline, shadow);
	}

	float get_text_height(Font font, float size)
	{
		return HUD::_GET_TEXT_SCALE_HEIGHT(size, static_cast<int>(font));
	}
};

inline std::unique_ptr<UIManagement> g_UIManagement;