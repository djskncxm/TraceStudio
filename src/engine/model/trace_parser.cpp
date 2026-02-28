#include <cstdint>
#include <vector>
#include <sstream>
#include <cassert>
#include <cctype>
#include <algorithm>
#include <iostream>

#include "trace_parser.h"

std::string trim(const std::string &s)
{
	auto isNotSpace = [](unsigned char ch) { return !std::isspace(ch); };
	auto left	= std::find_if(s.begin(), s.end(), isNotSpace);
	auto right	= std::find_if(s.rbegin(), s.rend(), isNotSpace).base();
	return (left < right) ? std::string(left, right) : "";
}
static uint32_t CalcChangedMask(const RegistersARM64 &prev, const RegistersARM64 &curr)
{
	uint32_t mask = 0;

	// x0–x30
	for (int i = 0; i < 31; ++i) {
		if (prev.x[i] != curr.x[i]) {
			mask |= REG_BIT_X(i);
		}
	}

	// sp
	if (prev.sp != curr.sp) {
		mask |= REG_BIT_SP;
	}

	return mask;
}

TraceManager TraceParser::ParseBlock(const std::vector<std::string> &lines)
{
	TraceManager manager;
	TraceLine    prevLine;
	bool	     hasPrev = false;

	for (const std::string &line : lines) {
		TraceLine traceLine = ParseLine(line);

		if (hasPrev) {
			traceLine.reg_changed_mask = CalcChangedMask(prevLine.Register, traceLine.Register);
		} else {
			traceLine.reg_changed_mask = 0;
		}

		manager.Instructions.push_back(std::move(traceLine));
		prevLine = manager.Instructions.back();
		hasPrev	 = true;
	}

	if (!manager.Instructions.empty()) {
		manager.loaded_begin = manager.Instructions.front().Line;
		manager.loaded_end   = manager.Instructions.back().Line;
	}

	return manager;
}

TraceManager &TraceManager::updateBlock(const TraceManager &other)
{
	if (this == &other) {
		throw std::invalid_argument("Cannot append TraceManager to itself");
	}

	this->PrevLine	   = this->Instructions;
	this->Instructions = other.Instructions;
	this->loaded_begin = other.loaded_begin;
	this->loaded_end   = other.loaded_end;
	if (!this->PrevLine.empty() && !this->Instructions.empty()) {
		this->Instructions[0].reg_changed_mask = CalcChangedMask(this->PrevLine.back().Register, this->Instructions[0].Register);
	}

	return *this;
}

TraceLine TraceParser::ParseLine(const std::string &line)
{
	TraceLine		 tl;
	std::vector<std::string> fields;
	std::string		 field;
	std::istringstream	 iss(line);

	while (std::getline(iss, field, '|')) {
		field = trim(field);

		if (!field.empty() && field.front() == '"' && field.back() == '"') {
			field = field.substr(1, field.size() - 2);
		}

		fields.push_back(std::move(field));
	}

	if (fields.size() < 5)
		return tl; // 防御

	tl.Line	       = std::stoul(fields[0], nullptr, 16);
	tl.Address     = std::stoull(fields[1], nullptr, 16);
	tl.Offset      = std::stoull(fields[2], nullptr, 16);
	tl.Description = fields[3];

	// ---------- 寄存器 ----------
	std::vector<std::string> reg_text(fields.begin() + 4, fields.end());
	tl.Register = ParseRegisters(reg_text);

	return tl;
}

static uint64_t parse_u64(const std::string &s)
{
	return std::stoull(s, nullptr, 16);
}

RegistersARM64 TraceParser::ParseRegisters(std::vector<std::string> &reg_text)
{
	assert(reg_text.size() >= 33);
	RegistersARM64 regs{};

	size_t i = 0;

	// x0–x30
	for (; i < 31 && i < reg_text.size(); ++i) {
		regs.x[i] = parse_u64(reg_text[i]);
	}

	// sp
	if (i < reg_text.size())
		regs.sp = parse_u64(reg_text[i++]);

	// pc
	if (i < reg_text.size())
		regs.pc = parse_u64(reg_text[i++]);

	// pstate
	if (i < reg_text.size())
		regs.pstate = static_cast<uint32_t>(parse_u64(reg_text[i++]));
	return regs;
}
