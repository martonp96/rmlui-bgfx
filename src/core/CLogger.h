#pragma once
#include "pch.h"
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/rotating_file_sink.h>

namespace core
{
    class CLogger
    {
    private:
        std::shared_ptr<spdlog::sinks::stdout_color_sink_mt> m_console_sink;
        std::shared_ptr<spdlog::sinks::rotating_file_sink_mt> m_file_sink;

    public:
        CLogger()
        {
            std::vector<spdlog::sink_ptr> sinks;

            m_console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
            m_console_sink->set_level(static_cast<spdlog::level::level_enum>(SPDLOG_ACTIVE_LEVEL));
            sinks.push_back(m_console_sink);

            m_file_sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>("logs/app.log", SIZE_MAX, 5, true);
            m_file_sink->set_level(spdlog::level::off);
            sinks.push_back(m_file_sink);

            const auto logger = std::make_shared<spdlog::logger>("default", begin(sinks), end(sinks));
            logger->set_pattern("[%H:%M:%S] [%^%l%$] %v");
            spdlog::set_default_logger(logger);
            spdlog::flush_every(std::chrono::seconds(1));
            SetConsoleOutputCP(65001);
        }

        ~CLogger()
        {
            spdlog::shutdown();
        }

        void set_console_log_level(const spdlog::level::level_enum log_level) const
        {
            m_console_sink->set_level(log_level);
        }

        void set_file_log_level(const spdlog::level::level_enum log_level) const
        {
            m_file_sink->set_level(log_level);
        }
    };
}
