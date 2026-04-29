#include "log.hpp"

#include <ml/abort.h>
#include <ml/string.h>

#include "../../ml/hw/eeuart.h"

const char* chaosGetLogStamp(ChaosLogLevel level) {
	switch(level) {
		case LogInfo:
			return "[Chaos Info]";
		case LogWarn:
			return "[Chaos Warn]";
		case LogErr:
			return "[Chaos ERROR]";
		default:
			mlASSERT(false);
			break;
	}
}

void chaosLog(ChaosLogLevel level, const char* pszMessage) {
	char buf[512];
	sprintf(&buf[0], "%s %s", chaosGetLogStamp(level), pszMessage);
	eeUartPuts(&buf[0]);
}

void chaosLogvf(ChaosLogLevel level, const char* pszMessage, va_list val) {
	char formatbuf[256];
	vsprintf(&formatbuf[0], pszMessage, val);
	chaosLog(level, &formatbuf[0]);
}

void chaosLogf(ChaosLogLevel level, const char* pszMessage, ...) {
	va_list val;
	va_start(val, pszMessage);
	chaosLogvf(level, pszMessage, val);
	va_end(val);
}
