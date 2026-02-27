// engine/parser/trace_parser.h
#pragma once
#include <string>
#include <vector>

#include <string>
#include <vector>
#include <cstdint>

struct TraceManger
{
	int			 id;
	std::string		 type;
	std::string		 description;
	uint64_t		 timestamp;
	std::vector<std::string> details; // 可用于表格或树节点显示
};

struct TraceLine
{
};

class TraceParser
{
    public:
	// 解析单行日志
	TraceManger parseLine(const std::string &line);

	// 解析一批日志
	std::vector<TraceManger> parseLines(const std::vector<std::string> &lines);
};
