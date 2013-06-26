#ifndef _LOGINTC_GLOBALS_H_
#define _LOGINTC_GLOBALS_H_

#include <stdlib.h>

enum logintc_status_t {
    LOGINTC_STATUS_OK, LOGINTC_STATUS_ERROR
};

enum logintc_client_error_type_t {
    LOGINTC_CLIENT_CURL_ERR, LOGINTC_CLIENT_API_ERR
};

enum logintc_session_state_type_t {
    LOGINTC_SESSION_PENDING, LOGINTC_SESSION_APPROVED, LOGINTC_SESSION_DENIED
};

typedef struct logintc_t {
    char* api_url;
    char* api_key;
    char* user_agent;
} logintc_t;

typedef struct logintc_response_t {
    int http_code;
    char* body;
} logintc_response_t;

typedef struct logintc_error_t {
    int error_type;
    int error_code;
    char* error_message;
} logintc_error_t;

typedef struct logintc_response_body_t {
    char* body;
    size_t size;
} logintc_response_body_t;

typedef struct logintc_session_t {
    char* id;
    int state;
} logintc_session_t;

#endif
