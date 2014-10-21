#ifndef _LOGINTC_ERROR_H_
#define _LOGINTC_ERROR_H_

#include <logintc/globals.h>

logintc_error_t* logintc_error_new();

void logintc_error_free(logintc_error_t* error);

#endif
