#include <memory>

#include "engine.h"
#include "../io/file_reader.h"
#include "../parser/trace_parser.h"

void Engine::parseFileSync(std::string FilePath)
{
	currentFile = FilePath;
	reader	    = std::make_unique<IFileReader>(FilePath);
	std::vector<std::string> lines;

	size_t linesToRead = std::min<size_t>(reader->TotalLines(), 3000);
	lines		   = reader->ReadBlock(0, linesToRead);

	if (lines.empty()) {
		throw std::runtime_error("Failed to read any lines from file: " + FilePath);
	}

	TraceParser tp;
	tm	    = tp.ParseBlock(lines);
	tm.FileName = FilePath;
}

void Engine::parseFileIncremental()
{
	if (currentFile.empty())
		throw std::runtime_error("No file loaded. Call parseFileSync first.");

	size_t totalLines = reader->TotalLines();
	if (tm.loaded_end >= totalLines)
		return; // 已经解析完

	size_t			 readEnd = std::min(tm.loaded_end + windowSize, totalLines);
	std::vector<std::string> lines	 = reader->ReadBlock(tm.loaded_end, readEnd);

	if (lines.empty())
		return;

	TraceParser  tp;
	TraceManager newBlock = tp.ParseBlock(lines);
	tm.updateBlock(newBlock);
}
