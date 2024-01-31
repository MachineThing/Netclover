#ifndef LOGGER_H
#define LOGGER_H

enum LogLevel {
    LOG_NORMAL,
    LOG_WARNING,
    LOG_ERROR,
    LOG_CRITICAL
};

void log(enum LogLevel level, const char* service, const char* format, ...);

#endif
