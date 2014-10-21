#include <logintc/globals.h>
#include <logintc/response.h>

#include <string.h>
#include <stdlib.h>

logintc_response_t*
logintc_response_new() {
    logintc_response_t* response;
    response = (logintc_response_t*) malloc(sizeof(logintc_response_t));
    response->body = NULL;
    return response;
}

void logintc_response_free(logintc_response_t* response) {
    if (response) {
        if (response->body != NULL) {
            free(response->body);
        }
    }

    free(response);
}
