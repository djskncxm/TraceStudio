#include "gui_init.h"
#include "config.def.h"
#include "imgui.h"
#include <GLFW/glfw3.h>

void SetupGuiThemeAndFonts()
{
	// 主题
	ImGui::StyleColorsDark();
	ImGuiStyle &style		= ImGui::GetStyle();
	style.Colors[ImGuiCol_WindowBg] = CATPPUCCIN_BG;
	style.Colors[ImGuiCol_Button]	= CATPPUCCIN_BUTTON;

	// 字体
	ImGuiIO &io = ImGui::GetIO();
	io.Fonts->AddFontFromFileTTF(DEFAULT_FONT_EN_PATH, DEFAULT_FONT_SIZE);

	ImFontConfig font_cfg;
	font_cfg.MergeMode		      = true;
	static const ImWchar chinese_ranges[] = { 0x4E00, 0x9FFF, 0 };
	io.Fonts->AddFontFromFileTTF(DEFAULT_FONT_ZH_PATH, DEFAULT_FONT_SIZE, &font_cfg, chinese_ranges);
}
