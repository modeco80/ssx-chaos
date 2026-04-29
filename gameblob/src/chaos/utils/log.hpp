#ifndef CHAOS_UTILS_LOG_HPP
#define CHAOS_UTILS_LOG_HPP

enum ChaosLogLevel {
	LogInfo,
	LogWarn,
	LogErr
};

void chaosLog(ChaosLogLevel level, const char* pszMessage);

void chaosLogf(ChaosLogLevel level, const char* pszMessage, ...);

#endif
