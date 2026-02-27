// 作者只有能力完成Linux的文件读取，引入此接口
#pragma once

#include <fstream>
#include <string>
#include <vector>

class IFileReader
{
    public:
	IFileReader(const std::string &filename)
		: istream(filename)
	{
		if (!istream.is_open()) {
			throw std::runtime_error("无法打开文件: " + filename);
		}
	}

	~IFileReader()
	{
		istream.close();
	}

	std::string		 ReadLine();
	std::vector<std::string> ReadBlock(int from, int to);

    private:
	std::ifstream istream;
};
