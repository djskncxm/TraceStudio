#pragma once

#include <GLFW/glfw3.h>
// 初始化主题和字体
void SetupGuiThemeAndFonts();

GLFWwindow *InitGLFW();
void	    InitImGui(GLFWwindow *window);

void DrawMainWindow();

void RenderFrame(GLFWwindow *window);

void Cleanup(GLFWwindow *window);

void DrawScaledText(const char *text, float windowWidth, float scale = 1.5f, float divisor = 6.0f);
