#include "gui_init.h"
#include "config.def.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "tinyfiledialogs.h"

#include <iostream>
#include <GLFW/glfw3.h>
#include <string>

enum class AppPage
{
	StartupPage, // 启动页
	WorkPage // 工作页
};

struct AppState
{
	AppPage	    currentPage = AppPage::StartupPage;
	std::string currentFile; // 当前打开的文件路径
};

// 全局状态（实际项目可封装为单例或上下文）
static AppState g_appState;

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

void SetupMainWindow()
{
	ImGuiViewport *viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->WorkPos);
	ImGui::SetNextWindowSize(viewport->WorkSize);
}

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
void DrawFileMenu()
{
	if (!ImGui::BeginMenu("File"))
		return;

	if (ImGui::MenuItem("Open", "Ctrl+O")) {
		std::string fn = GetFileName();
		if (!fn.empty()) {
			g_appState.currentFile = fn;
			g_appState.currentPage = AppPage::WorkPage; // 打开文件后自动切换到工作页
		}
	}

	if (ImGui::MenuItem("Save", "Ctrl+S")) {
	}

	// if (ImGui::MenuItem("Exit")) { }

	ImGui::EndMenu();
}

void DrawEditMenu()
{
	if (ImGui::BeginMenu("Edit")) // Edit 菜单
	{
		if (ImGui::MenuItem("Copy", "Ctrl+C")) { /* 复制逻辑 */
		}
		if (ImGui::MenuItem("Paste", "Ctrl+V")) { /* 粘贴逻辑 */
		}
		ImGui::EndMenu();
	}
}
void DrawHelpMenu()
{
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
}

void DrawMainMenuBar()
{
	if (!ImGui::BeginMenuBar())
		return;

	DrawFileMenu();
	DrawEditMenu();
	DrawHelpMenu();

	ImGui::EndMenuBar();
}
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
void DrawOpenFileButton()
{
	float availWidth = ImGui::GetContentRegionAvail().x;
	ImGui::SetCursorPosX((availWidth - 200) * 0.5f);

	if (StyledButton("选择文件", ImVec2(200, 60))) {
		std::string selectedFile = GetFileName();

		if (!selectedFile.empty()) {
			g_appState.currentFile = selectedFile;
			g_appState.currentPage = AppPage::WorkPage; // 切换到工作页
		}
	}
}
void DrawLeftPanel(float window_width)
{
	ImGui::Dummy(ImVec2(0, 20));
	DrawScaledText("TraceStudio", window_width);
	ImGui::Dummy(ImVec2(0, 30));

	DrawOpenFileButton();
}

void DrawWorkPanel()
{
}

void DrawMainLayout()
{
	float window_width = ImGui::GetContentRegionAvail().x;
	if (g_appState.currentPage == AppPage::StartupPage) {
		ImGui::BeginChild("LeftPanel", ImVec2(window_width / 6, 0), true);
		DrawLeftPanel(window_width); // 注意传入的是主窗口宽度，用于居中文字
		ImGui::EndChild();

		ImGui::SameLine(); // 左右并排

		// 右侧面板 (占剩余宽度)
		ImGui::BeginChild("RightPanel", ImVec2(0, 0), true);
		DrawStartupRightPanel();
		ImGui::EndChild();
	} else if (g_appState.currentPage == AppPage::WorkPage) {
		std::cout << "工作分区" << '\n';
	}
}

void DrawMainWindow()
{
	SetupMainWindow();

	if (ImGui::Begin("Main Window", nullptr, GetMainWindowFlags())) {
		DrawMainMenuBar();
		DrawMainLayout();
	}

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
// 打开文件管理器获取文件
std::string GetFileName()
{
	std::string selectedFile;
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
	return selectedFile;
}

void DrawStartupRightPanel()
{
	float availWidth  = ImGui::GetContentRegionAvail().x;
	float availHeight = ImGui::GetContentRegionAvail().y;

	// 临时放大字体（例如 1.5 倍）
	ImGui::SetWindowFontScale(1.5f);

	const char *text = "欢迎使用 TraceStudio\n请从左侧打开一个文件，或使用菜单 File -> Open 开始工作。";
	// 计算文本实际占用的宽高（考虑自动换行，限制最大宽度为 availWidth）
	ImVec2 textSize = ImGui::CalcTextSize(text, nullptr, false, availWidth);

	// 设置光标位置使文本居中
	ImGui::SetCursorPos(ImVec2((availWidth - textSize.x) * 0.5f, (availHeight - textSize.y) * 0.5f));

	// 绘制文本（自动换行）
	ImGui::TextWrapped("%s", text);

	// 恢复字体缩放，避免影响后续绘制
	ImGui::SetWindowFontScale(1.0f);
}
