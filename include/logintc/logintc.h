#ifndef _LOGINTC_H_
#define _LOGINTC_H_

#include <logintc/globals.h>

static const char *LOGINTC_NAME = "LoginTC-C";
static const char *LOGINTC_VERSION = "0.1.0";
static const char *LOGINTC_API_URL = "https://cloud.logintc.com";
static const char *LOGINTC_CONTENT_TYPE = "application/vnd.logintc.v1+json";

logintc_t* logintc_logintc_new(char const* const api_key);

void logintc_logintc_free(logintc_t* logintc);

logintc_session_t* logintc_create_session_with_username(
        logintc_t const* const logintc, char const* const domain_id,
        char const* const user_name, logintc_error_t* error);

void logintc_get_session(logintc_t const* const logintc,
        char const* const domain_id, logintc_session_t** session);

#endif
