#include "engine.h"
#include "file_reader.h"
#include <iostream>

Engine::Engine()
{
	std::cout << "Engine 构造函数被调用\n";
}

Engine::~Engine()
{
	std::cout << "Engine 构造函数被析构\n";
}

void Engine::test()
{
	IFileReader reader("../../assets/trace_logs/code.log");

	std::vector<std::string> lines = reader.ReadBlock(0, 100000); // 读取第 5~10 行

	for (std::string &l : lines) {
		std::cout << l << "\n";
	}
}
int main(int argc, char *argv[])
{
	Engine *e = new Engine();
	e->test();

	delete e;
	return 0;
}
