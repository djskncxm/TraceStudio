#pragma once

#include <functional>
#include <vector>
#include <string>
#include "file_reader.h"

#include "model/trace_parser.h"

class Engine
{
    public:
	Engine();
	~Engine();
	void			 parseFileAsync(IFileReader *reader, size_t batchSize, std::function<void(const std::vector<TraceManger> &)> onBatch, std::function<void(const std::string &)> onError);
	std::vector<TraceManger> parseFileSync(IFileReader *reader);
	void			 test();
};
