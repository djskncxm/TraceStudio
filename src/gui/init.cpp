#include "init.h"
#include "AppState.h"
#include "work.h"
#include "config.def.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "tinyfiledialogs.h"

#include <iostream>
#include <GLFW/glfw3.h>
#include <string>

AppState g_appState;
// ===== 全局函数实现 =====

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

GLFWwindow *InitGLFW()
{
	if (!glfwInit())
		return nullptr;
	GLFWwindow *window = glfwCreateWindow(1280, 720, "TraceStudio", nullptr, nullptr);
	if (!window) {
		glfwTerminate();
		return nullptr;
	}
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);
	return window;
}

void InitImGui(GLFWwindow *window)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO &io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 130");

	SetupGuiThemeAndFonts();
}

void DrawMainWindow()
{
	PrepareMainWindowPosSize();

	if (ImGui::Begin("Main Window", nullptr, GetMainWindowFlags())) {
		DrawMainMenuBar();
		DrawMainLayout();
	}
	ImGui::End();
}

void RenderFrame(GLFWwindow *window)
{
	ImGui::Render();
	int display_w, display_h;
	glfwGetFramebufferSize(window, &display_w, &display_h);
	glViewport(0, 0, display_w, display_h);
	glClearColor(0.12f, 0.11f, 0.10f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	ImGuiIO &io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
		GLFWwindow *backup_current_context = glfwGetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		glfwMakeContextCurrent(backup_current_context);
	}

	glfwSwapBuffers(window);
}

void Cleanup(GLFWwindow *window)
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	glfwDestroyWindow(window);
	glfwTerminate();
}

void DrawScaledText(const char *text, float windowWidth, float scale, float divisor)
{
	ImGui::SetWindowFontScale(scale);
	float textWidth = ImGui::CalcTextSize(text).x;
	float xPos	= (windowWidth / divisor - textWidth) * 0.5f;
	ImGui::SetCursorPosX(xPos);
	ImGui::Text("%s", text);
	ImGui::SetWindowFontScale(1.0f);
}

std::string GetFileName()
{
	const char *filename = tinyfd_openFileDialog(
		"选择文件", // 对话框标题
		"", // 默认路径
		0,
		NULL,
		NULL,
		0); // 过滤器（不限制）
	if (filename) {
		std::cout << "选择的文件: " << filename << "\n";
		return filename;
	}
	return "";
}
