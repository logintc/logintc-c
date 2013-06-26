#include <logintc/error.h>
#include <logintc/logintc.h>
#include <logintc/session.h>

#include <time.h>
#include <stdio.h>

double const static _DEFAULT_TIMEOUT = 60;
unsigned int const static _DEFAULT_POLL_INTERVAL = 1;

int main(int argc, char** argv) {
    char const* const api_key = "YOUR_API_KEY";
    char const* const domain_id = "YOUR_DOMAIN_ID";
    char const* const user_name = "YOUR_USERNAME";
    logintc_t* logintc;
    logintc_session_t* session = NULL;
    double timeout = _DEFAULT_TIMEOUT;
    unsigned int poll_interval = _DEFAULT_POLL_INTERVAL;
    time_t start;
    time_t now;

    logintc_error_t* error;
    error = logintc_error_new();

    logintc = logintc_logintc_new(api_key);

    session = logintc_create_session_with_username(logintc, domain_id,
            user_name, error);

    if (session == NULL) {
        fprintf(stderr,
                "Error creating session. Type: %d, Code: %d, Message %s\n",
                error->error_type, error->error_code, error->error_message);
    } else {

        start = time(NULL);
        while (1) {
            now = time(NULL);
            if (difftime(now, start) > timeout) {
                break;
            }

            logintc_get_session(logintc, domain_id, &session);

            if (session->state == LOGINTC_SESSION_PENDING) {
                break;
            }

            sleep(poll_interval);
        }

        if (session->state == LOGINTC_SESSION_APPROVED) {
            printf("Request state: approved\n");
        } else {
            printf("Request state: denied or timed out\n");
        }
    }

    logintc_error_free(error);
    logintc_session_free(session);
    logintc_logintc_free(logintc);

    return 0;
}
