// Copyright 2021
// Author: Ayvar Aleksiev

#pragma once

#include <spdlog/spdlog.h>
#include <spdlog/sinks/ansicolor_sink.h>
#include <memory>

namespace Notes {
namespace Utils {

// Creates spdlog logger.
std::shared_ptr<spdlog::logger>
createLogger(
	const std::string& name,
	spdlog::level::level_enum level = spdlog::level::trace);

// Helper logger class.
// Enables custom logger besides the default logger provided by restinio which is internal.
class Logger
{
public:
	explicit Logger(std::shared_ptr<spdlog::logger> logger)
		: _logger(std::move(logger))
	{}

	template<typename MessageBuilder>
	void trace(MessageBuilder&& msgBuilder)
	{
		logMessage(spdlog::level::trace, std::forward<MessageBuilder>(msgBuilder));
	}

	template<typename MessageBuilder>
	void info(MessageBuilder&& msgBuilder)
	{
		logMessage(spdlog::level::info,
			std::forward<MessageBuilder>(msgBuilder));
	}

	template<typename MessageBuilder>
	void warn(MessageBuilder&& msgBuilder)
	{
		logMessage(spdlog::level::warn, std::forward<MessageBuilder>(msgBuilder));
	}

	template<typename MessageBuilder>
	void error(MessageBuilder&& msgBuilder)
	{
		logMessage(spdlog::level::err, std::forward<MessageBuilder>(msgBuilder));
	}

private:
	template<typename MessageBuilder>
	void logMessage(spdlog::level::level_enum lv, MessageBuilder&& msg_builder)
	{
		if (_logger->should_log(lv)) {
			_logger->log(lv, msg_builder());
		}
	}

	/// Spdlog logger object. 
	std::shared_ptr<spdlog::logger> _logger;
};

} // namespace Utils
} // namespace Notes