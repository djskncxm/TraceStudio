// 作者只有能力完成Linux的文件读取，引入此接口
#pragma once

#include <fstream>
#include <string>
#include <vector>
#include <cstdint>

class IFileReader
{
    public:
	IFileReader(const std::string &filename)
		: istream(filename)
	{
		if (!istream.is_open()) {
			throw std::runtime_error("无法打开文件: " + filename);
		}
		BuildOffsetTable();
	}

	~IFileReader()
	{
		istream.close();
	}

	std::vector<std::string> ReadBlock(uint64_t from, uint64_t to);
	size_t			 TotalLines() const
	{
		return line_offsets.size();
	}

    private:
	std::vector<uint64_t> line_offsets;
	std::ifstream	      istream;
	void		      BuildOffsetTable();
};
