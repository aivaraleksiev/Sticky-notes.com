// Copyright 2021
// Author: Ayvar Aleksiev

#include "Logger.h"

namespace Notes {
namespace Utils {

namespace {

spdlog::sink_ptr
createLoggerSink()
{
	auto sink = std::make_shared< spdlog::sinks::ansicolor_stdout_sink_mt>();
	sink->set_pattern("[%H:%M:%S %z][ProcId %P][Thread %t][%^%l%$][sub=%n] %v");
	return sink;
}

} // anonymous namespace 

std::shared_ptr<spdlog::logger>
createLogger( std::string const& name,	 spdlog::level::level_enum level)
{
	auto sink = createLoggerSink();
	auto logger = std::make_shared<spdlog::logger>(name, std::move(sink));
	logger->set_level(level);
	logger->flush_on(level);
	return logger;
}

} // namespace Utils
} // namespace Notes