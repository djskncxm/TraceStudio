#include "gui/gui_init.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>

// ------------------------------
// 初始化 GLFW / OpenGL
// ------------------------------
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

// ------------------------------
// 初始化 ImGui
// ------------------------------
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

// ------------------------------
// 绘制主窗口
// ------------------------------
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
		ImGui::EndMenuBar(); // 结束菜单栏
	}

	// 计算宽度
	float window_width = ImGui::GetContentRegionAvail().x;
	float child_width  = window_width / 3.0f;

	// 左侧
	ImGui::BeginChild("Left", ImVec2(child_width, 0), true);
	for (int i = 1; i <= 100; ++i) {
		ImGui::Text("Item %d", i); // 显示文本
	}
	ImGui::EndChild();
	ImGui::SameLine();

	// 中间
	ImGui::BeginChild("Center", ImVec2(child_width, 0), true);
	ImGui::Text("中间内容");
	ImGui::EndChild();
	ImGui::SameLine();

	// 右侧
	ImGui::BeginChild("Right", ImVec2(0, 0), true); // 最后一个用剩余空间
	ImGui::Text("右侧内容");
	ImGui::EndChild();

	ImGui::End();
}

// ------------------------------
// 渲染
// ------------------------------
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

// ------------------------------
// 主函数
// ------------------------------
int main()
{
	GLFWwindow *window = InitGLFW();
	if (!window)
		return -1;

	InitImGui(window);

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		DrawMainWindow();

		RenderFrame(window);
	}

	Cleanup(window);
}
