#include "init.h"
#include "AppState.h"
#include "imgui.h"
#include <iostream>

void OpenFileAndSwitchToWorkPage()
{
	std::string filename = GetFileName();
	if (!filename.empty()) {
		g_appState.currentFile = filename;
		g_appState.currentPage = AppPage::WorkPage;
	}
}

// 设置主窗口位置和大小为视口工作区
void PrepareMainWindowPosSize()
{
	ImGuiViewport *viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->WorkPos);
	ImGui::SetNextWindowSize(viewport->WorkSize);
}

// 获取主窗口标志
ImGuiWindowFlags GetMainWindowFlags()
{
	ImGuiWindowFlags flags = ImGuiWindowFlags_MenuBar;
	flags |= ImGuiWindowFlags_NoTitleBar;
	flags |= ImGuiWindowFlags_NoCollapse;
	flags |= ImGuiWindowFlags_NoResize;
	flags |= ImGuiWindowFlags_NoMove;
	flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
	flags |= ImGuiWindowFlags_NoNavFocus;
	return flags;
}

// 绘制 File 菜单
void DrawFileMenu()
{
	if (!ImGui::BeginMenu("File"))
		return;

	if (ImGui::MenuItem("Open", "Ctrl+O")) {
		OpenFileAndSwitchToWorkPage();
	}

	if (ImGui::MenuItem("Save", "Ctrl+S")) {
		// TODO: 保存逻辑
	}

	ImGui::EndMenu();
}

// 绘制 Edit 菜单
void DrawEditMenu()
{
	if (ImGui::BeginMenu("Edit")) {
		if (ImGui::MenuItem("Copy", "Ctrl+C")) { /* 复制逻辑 */
		}
		if (ImGui::MenuItem("Paste", "Ctrl+V")) { /* 粘贴逻辑 */
		}
		ImGui::EndMenu();
	}
}

// 绘制 Help 菜单
void DrawHelpMenu()
{
	if (ImGui::BeginMenu("帮助")) {
		if (ImGui::MenuItem("使用教程")) { /* 教程 */
		}
		if (ImGui::MenuItem("提交PR")) { /* 提交PR */
		}
		if (ImGui::MenuItem("提交需求")) { /* 提交需求 */
		}
		ImGui::EndMenu();
	}
}

// 绘制主菜单栏
void DrawMainMenuBar()
{
	if (!ImGui::BeginMenuBar())
		return;

	DrawFileMenu();
	DrawEditMenu();
	DrawHelpMenu();

	ImGui::EndMenuBar();
}

// 带样式的按钮
bool StyledButton(const char *label, ImVec2 size)
{
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.263f, 0.278f, 0.282f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.313f, 0.328f, 0.332f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.213f, 0.228f, 0.232f, 1.0f));
	ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 7.0f);

	bool clicked = ImGui::Button(label, size);

	ImGui::PopStyleColor(3);
	ImGui::PopStyleVar();

	return clicked;
}

// 绘制“选择文件”按钮
void DrawOpenFileButton()
{
	float availWidth = ImGui::GetContentRegionAvail().x;
	ImGui::SetCursorPosX((availWidth - 200) * 0.5f);

	if (StyledButton("选择文件", ImVec2(200, 60))) {
		OpenFileAndSwitchToWorkPage();
	}
}

// 绘制左侧面板
void DrawLeftPanel(float window_width)
{
	ImGui::Dummy(ImVec2(0, 20));
	DrawScaledText("TraceStudio", window_width);
	ImGui::Dummy(ImVec2(0, 30));
	DrawOpenFileButton();
}

// 绘制工作页内容（待实现）
void DrawWorkPanel()
{
	// TODO: 工作区内容
	std::cout << "工作分区" << '\n';
}

// 绘制启动页布局
void DrawStartPanel(float window_width)
{
	// 左侧面板（占 1/6 宽度）
	ImGui::BeginChild("LeftPanel", ImVec2(window_width / 6, 0), true);
	DrawLeftPanel(window_width);
	ImGui::EndChild();

	ImGui::SameLine();

	// 右侧面板
	ImGui::BeginChild("RightPanel", ImVec2(0, 0), true);
	float availWidth  = ImGui::GetContentRegionAvail().x;
	float availHeight = ImGui::GetContentRegionAvail().y;

	ImGui::SetWindowFontScale(1.5f);
	const char *text     = "欢迎使用 TraceStudio\n请从左侧打开一个文件，或使用菜单 File -> Open 开始工作。";
	ImVec2	    textSize = ImGui::CalcTextSize(text, nullptr, false, availWidth);
	ImGui::SetCursorPos(ImVec2((availWidth - textSize.x) * 0.5f, (availHeight - textSize.y) * 0.5f));
	ImGui::TextWrapped("%s", text);
	ImGui::SetWindowFontScale(1.0f);
	ImGui::EndChild();
}

// 根据当前页面绘制主布局
void DrawMainLayout()
{
	float window_width = ImGui::GetContentRegionAvail().x;
	if (g_appState.currentPage == AppPage::StartupPage) {
		DrawStartPanel(window_width);
	} else {
		DrawWorkPanel();
	}
}
