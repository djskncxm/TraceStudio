#pragma once

#include <string>
#include <vector>
#include <cstdint>

#define REG_BIT_X(i) (1u << (i))
#define REG_BIT_SP (1u << 31)

struct RegistersARM64
{
	uint64_t x[31]; // x0–x30
	uint64_t sp;
	uint64_t pc;
	uint32_t pstate;
};

struct TraceLine
{
	int	       Line;
	uint64_t       Offset;
	uint64_t       Address;
	std::string    Description;
	uint32_t       reg_changed_mask;
	RegistersARM64 Register;
};

struct TraceManager
{
	std::vector<TraceLine> Instructions;
	std::vector<TraceLine> PrevLine;
	int		       CurrentIndex;
	uint64_t	       loaded_begin;
	uint64_t	       loaded_end;
	std::string	       FileName;
	int		       windowSize;
	bool		       isLoading;
	TraceManager	      &updateBlock(const TraceManager &other);
};

class TraceParser
{
    public:
	TraceParser() = default;
	TraceManager ParseBlock(const std::vector<std::string> &lines);

    private:
	TraceLine      ParseLine(const std::string &line);
	RegistersARM64 ParseRegisters(std::vector<std::string> &reg_text);
};
