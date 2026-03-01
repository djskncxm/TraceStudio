#pragma once

#include <string>
#include <GLFW/glfw3.h>

// 初始化 ImGui 主题和字体
void SetupGuiThemeAndFonts();

// 初始化 GLFW，创建窗口
GLFWwindow *InitGLFW();

// 初始化 ImGui 上下文和后端
void InitImGui(GLFWwindow *window);

// 绘制主窗口（应在每一帧调用）
void DrawMainWindow();

// 渲染 ImGui 帧并交换缓冲区
void RenderFrame(GLFWwindow *window);

// 清理资源
void Cleanup(GLFWwindow *window);

// 辅助函数：在窗口中绘制缩放文本，根据窗口宽度自动水平居中（相对于左面板宽度 divisor 分之一）
void DrawScaledText(const char *text, float windowWidth, float scale = 1.5f, float divisor = 6.0f);

// 打开文件对话框，返回选择的文件路径，空字符串表示取消
std::string GetFileName();
