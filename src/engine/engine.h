#pragma once

#include <memory>
#include "file_reader.h"
#include "model/trace_parser.h"
#include <string>

class Engine
{
    public:
	int windowSize = 3000;
	Engine()       = default;
	~Engine()      = default;
	void parseFileSync(std::string FilenPath);

	void		    parseFileIncremental();
	const TraceManager &getTraceManager() const
	{
		return tm;
	}

    private:
	TraceManager		     tm;
	std::unique_ptr<IFileReader> reader;
	std::string		     currentFile;
	size_t			     currentLine = 0; // 已解析的行数
};
