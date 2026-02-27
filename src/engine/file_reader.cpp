#include "file_reader.h"
#include <string>

std::string IFileReader::ReadLine()
{
	std::string line;
	if (std::getline(istream, line))
		return line;
	else
		return "";
}

std::vector<std::string> IFileReader::ReadBlock(int from, int to)
{
	if (from > to || from < 0) {
		throw std::invalid_argument("行区间不合法");
	}

	std::vector<std::string> result;
	istream.clear();
	istream.seekg(0, std::ios::beg);

	std::string line;
	int	    line_num = 0;

	while (std::getline(istream, line)) {
		if (line_num >= from && line_num <= to) {
			result.push_back(line);
		}
		if (line_num > to) {
			break; // 已读完目标区间
		}
		++line_num;
	}

	return result;
}
