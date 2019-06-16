#include <cstdarg>
#include "Logger.h"

namespace utils {

    /* Global lock to ensure only one thread is writing to output. */
    std::mutex Logger::g_output_lock;

    /**
     * @param tag An identifier for the current logging object.
     * @param verbose Filtering mode for output messages.
     */
    Logger::Logger(std::string tag, const uint32_t verbose)
        : m_verbose(verbose)
        , m_tag(std::move(tag)) { }

    /**
     * Outputs a message to stdout. Format follows that of printf. This can
     * be filtered out based on the verbosity mode.
     *
     * @param format A string that gives the output format of the message.
     *  (See printf)
     * @param ... Variables to take values from according to format.
     * @see printf
     */
    void Logger::Info(const char *format, ...) const {
        if(m_verbose >= LOG_LEVEL_INFO) {
            g_output_lock.lock();
            printf("%s INFO: ", m_tag.c_str());
            va_list args;
            va_start(args, format);
            vfprintf(stdout, format, args);
            va_end(args);
            printf("\n");
            g_output_lock.unlock();
        }
    }

    /**
     * Outputs a message to stdout. Format follows that of printf. This can
     * be filtered out based on the verbosity mode.
     *
     * @param format A string that gives the output format of the message.
     *  (See printf)
     * @param ... Variables to take values from according to format.
     * @see printf
     */
    void Logger::Warn(const char *format, ...) const {
        if(m_verbose >= LOG_LEVEL_WARN) {
            g_output_lock.lock();
            printf("%s WARN: ", m_tag.c_str());
            va_list args;
            va_start(args, format);
            vfprintf(stdout, format, args);
            va_end(args);
            printf("\n");
            g_output_lock.unlock();
        }
    }

    /**
     * Outputs a message to stderr. Format follows that of printf. This can
     * be filtered out based on the verbosity mode.
     *
     * @param format A string that gives the output format of the message.
     *  (See printf)
     * @param ... Variables to take values from according to format.
     * @see printf
     */
    void Logger::Error(const char *format, ...) const {
        if(m_verbose >= LOG_LEVEL_ERROR) {
            g_output_lock.lock();
            printf("%s ERR: ", m_tag.c_str());
            va_list args;
            va_start(args, format);
            vfprintf(stderr, format, args);
            va_end(args);
            printf("\n");
            g_output_lock.unlock();
        }
    }
}
