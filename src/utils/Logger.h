/**
 * This is a thread safe logging component. It should be instantiated with a
 * verbosity mode based on the build type and used similar to how printf
 * works.
 *
 * Author: Daniel Simpkins
 * Date: 6/15/19
 */

#ifndef ASTRO_LOGGER_H
#define ASTRO_LOGGER_H

#include <string>
#include <mutex>

#define LOG_LEVEL_ERROR 0
#define LOG_LEVEL_WARN  1
#define LOG_LEVEL_INFO  2

namespace utils {
    class Logger {
    public:
        Logger(std::string tag, uint32_t verbose);

        void Info(const char *format, ...) const;
        void Warn(const char *format, ...) const;
        void Error(const char *format, ...) const;
    private:
        uint32_t            m_verbose;
        std::string         m_tag;
        static std::mutex   g_output_lock;
    };
}

#endif // ASTRO_LOGGER_H
