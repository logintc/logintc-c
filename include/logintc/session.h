#ifndef _LOGINTC_SESSION_H_
#define _LOGINTC_SESSION_H_

#include <logintc/globals.h>

static const char *LOGINTC_SESSION_PENDING_STR = "pending";
static const char *LOGINTC_SESSION_APPROVED_STR = "approved";
static const char *LOGINTC_SESSION_DENIED_STR = "denied";

logintc_session_t* logintc_session_new(const char* session_str);

void logintc_session_free(logintc_session_t* session);

#endif
