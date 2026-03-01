#include "file_reader.h"
#include <string>

std::vector<std::string> IFileReader::ReadBlock(uint64_t from, uint64_t to)
{
	if (from > to || from < 0 || to >= line_offsets.size()) {
		throw std::invalid_argument("行区间不合法");
	}

	std::vector<std::string> result(to - from + 1);

	istream.clear();
	istream.seekg(line_offsets[from], std::ios::beg); // 只 seek 一次

	for (size_t i = 0; i <= to - from; ++i) {
		std::getline(istream, result[i]);
	}

	return result;
}

void IFileReader::BuildOffsetTable()
{
	line_offsets.clear();
	istream.clear();
	istream.seekg(0, std::ios::beg);

	std::string line;
	uint64_t    offset = 0;

	while (std::getline(istream, line)) {
		line_offsets.push_back(offset);
		offset = istream.tellg();
	}
}
