#include <logintc/session.h>

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

char static*
_substring(char const* const s, size_t offset, size_t count) {
    char* out;

    if (count < 0 || offset < 0 || strlen(s) - offset < count) {
        return NULL;
    }

    out = (char*) malloc(count + 1);
    memset(out, 0, count + 1);
    memcpy(out, s + offset, count);
    return out;
}

logintc_session_t*
logintc_session_new(const char* session_str) {
    logintc_session_t* session = NULL;

    char* id;
    char* state;

    if (strstr(session_str, "{\"id\":")) {
        id = _substring(session_str, 7, 40);
        state = _substring(session_str, 58, strlen(session_str) - 58 - 2);

        session = (logintc_session_t*) malloc(sizeof(logintc_session_t));
        session->id = (char*) malloc(strlen(id) + 1);
        snprintf(session->id, strlen(id) + 1, "%s", id);

        if (strcmp(state, LOGINTC_SESSION_PENDING_STR) == 0) {
            session->state = LOGINTC_SESSION_PENDING;
        } else if (strcmp(state, LOGINTC_SESSION_APPROVED_STR) == 0) {
            session ->state = LOGINTC_SESSION_APPROVED;
        } else if (strcmp(state, LOGINTC_SESSION_DENIED_STR) == 0) {
            session->state = LOGINTC_SESSION_DENIED;
        } else {
            session->state = -1;
        }

        free(id);
        free(state);
    }

    return session;
}

void logintc_session_free(logintc_session_t* session) {
    if (session) {
        free(session->id);
        free(session);
    }
    session = NULL;
}
