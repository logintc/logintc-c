#include <logintc/globals.h>
#include <logintc/error.h>

#include <string.h>
#include <stdlib.h>

logintc_error_t*
logintc_error_new() {
    logintc_error_t* error;
    error = (logintc_error_t*) malloc(sizeof(logintc_error_t));
    error->error_message = NULL;
    return error;
}

void logintc_error_free(logintc_error_t* error) {
    if (error) {
        if (error->error_message != NULL) {
            free(error->error_message);
        }
    }

    free(error);
}
