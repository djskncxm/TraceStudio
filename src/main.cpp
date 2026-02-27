#include "gui/gui_init.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>

int main()
{
	// ------------------------------
	// 初始化 GLFW / OpenGL
	// ------------------------------
	if (!glfwInit())
		return -1;
	GLFWwindow *window = glfwCreateWindow(1280, 720, "TraceStudio", NULL, NULL);
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	// ------------------------------
	// 初始化 ImGui
	// ------------------------------
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO &io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable; // ✅ 启用 Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; // ✅ 可选：多窗口支持

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 130");

	// ------------------------------
	// 主题 + 字体
	// ------------------------------
	SetupGuiThemeAndFonts();

	// ------------------------------
	// 主循环
	// ------------------------------
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		// ------------------------------
		// 单主窗口 + DockSpace
		// ------------------------------
		ImGuiWindowFlags main_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		ImGuiViewport	*viewport   = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->WorkPos);
		ImGui::SetNextWindowSize(viewport->WorkSize);
		main_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		main_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

		ImGui::Begin("MainDockWindow", nullptr, main_flags);

		// 创建 DockSpace
		ImGuiID dockspace_id = ImGui::GetID("MainDockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);

		ImGui::End();

		// ------------------------------
		// 这里是子窗口 / 子面板
		// ------------------------------
		ImGui::Begin("Console");
		ImGui::Text("Hello, World!");
		ImGui::Text("你好，世界！");
		ImGui::End();

		ImGui::Begin("Properties");
		ImGui::Text("这里可以放属性面板");
		ImGui::End();

		// ------------------------------
		// 渲染
		// ------------------------------
		ImGui::Render();
		int display_w, display_h;
		glfwGetFramebufferSize(window, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);
		glClearColor(0.12f, 0.11f, 0.10f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		// 可选：多显示器支持
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
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	glfwDestroyWindow(window);
	glfwTerminate();
}
