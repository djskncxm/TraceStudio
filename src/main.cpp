#include "gui/init.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

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
