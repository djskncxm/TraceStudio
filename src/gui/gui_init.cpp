#include "gui_init.h"
#include "config.def.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "tinyfiledialogs.h"

#include <iostream>
#include <GLFW/glfw3.h>

// 设置主题和字体
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

// 后端初始化
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

// 初始化库
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

// 窗口绘制
void DrawMainWindow()
{
	ImGuiWindowFlags main_flags = ImGuiWindowFlags_MenuBar;
	ImGuiViewport	*viewport   = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->WorkPos);
	ImGui::SetNextWindowSize(viewport->WorkSize);
	main_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
	main_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

	ImGui::Begin("Main Window", nullptr, main_flags);

	if (ImGui::BeginMenuBar()) // 开始菜单栏
	{
		if (ImGui::BeginMenu("File")) // File 菜单
		{
			if (ImGui::MenuItem("Open", "Ctrl+O")) { /* 打开文件逻辑 */
			}
			if (ImGui::MenuItem("Save", "Ctrl+S")) { /* 保存逻辑 */
			}
			if (ImGui::MenuItem("Exit")) { /* 退出逻辑 */
			}
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Edit")) // Edit 菜单
		{
			if (ImGui::MenuItem("Copy", "Ctrl+C")) { /* 复制逻辑 */
			}
			if (ImGui::MenuItem("Paste", "Ctrl+V")) { /* 粘贴逻辑 */
			}
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("帮助")) // Edit 菜单
		{
			if (ImGui::MenuItem("使用教程")) { /* 复制逻辑 */
			}
			if (ImGui::MenuItem("提交PR")) {
			}
			if (ImGui::MenuItem("提交需求")) {
			}
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar(); // 结束菜单栏
	}

	// 计算宽度
	float window_width = ImGui::GetContentRegionAvail().x;

	ImGui::BeginChild("Left", ImVec2(window_width / 6, 0), true);
	ImGui::Dummy(ImVec2(0, 20));
	const char *text = "TraceStudio";
	DrawScaledText(text, window_width);
	ImGui::Dummy(ImVec2(0, 30));
	std::string selectedFile;

	if (ImGui::Button("选择文件")) {
		const char *filename = tinyfd_openFileDialog(
			"选择文件", // 对话框标题
			"", // 默认路径
			0,
			NULL,
			NULL,
			0); // 过滤器，这里不限制
		if (filename) {
			std::cout << "选择的文件: " << filename << "\n";
			selectedFile = filename;
		}
	}

	if (ImGui::Button("按钮2")) {
		// 点击按钮2的处理
		std::cout << "按钮2被点击\n";
	}

	ImGui::EndChild();

	ImGui::End();
}

// 渲染
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

// ------------------------------
// 清理
// ------------------------------
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
	// 推入当前字体（保持字体堆栈一致，避免影响外部字体设置）
	ImGui::PushFont(ImGui::GetFont());

	// 设置窗口字体缩放
	ImGui::SetWindowFontScale(scale);

	// 计算文本宽度
	float textWidth = ImGui::CalcTextSize(text).x;

	// 根据公式 (windowWidth/divisor - textWidth) * 0.5f 计算水平起始位置
	float xPos = (windowWidth / divisor - textWidth) * 0.5f;
	ImGui::SetCursorPosX(xPos);

	// 绘制文本
	ImGui::Text("%s", text);

	// 恢复字体缩放为1.0
	ImGui::SetWindowFontScale(1.0f);

	// 弹出字体，恢复之前的字体状态
	ImGui::PopFont();
}
