#include <string>
#include "../engine/engine.h"

enum class AppPage
{
	StartupPage, // 启动页
	WorkPage // 工作页
};

// 应用程序状态
struct AppState
{
	AppPage	    currentPage = AppPage::StartupPage;
	std::string currentFile;
	Engine	    engine;
};

extern AppState g_appState;
