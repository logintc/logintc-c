#include <curl/curl.h>
#include <logintc/globals.h>
#include <logintc/logintc.h>
#include <logintc/response.h>
#include <logintc/session.h>
#include <logintc/error.h>

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

static size_t _write_response_body_callback(void *contents, size_t size,
        size_t nmemb, void *userp) {

    size_t realsize = size * nmemb;
    struct logintc_response_body_t *response_body =
            (struct logintc_response_body_t *) userp;

    response_body->body = realloc(response_body->body,
            response_body->size + realsize + 1);

    if (response_body->body == NULL) {
        /* out of memory! */
        fprintf(stderr,
                "_write_response_body_callback: Not enough memory (realloc returned NULL)\n");
        exit(EXIT_FAILURE);
    }

    memcpy(&(response_body->body[response_body->size]), contents, realsize);
    response_body->size += realsize;
    response_body->body[response_body->size] = 0;

    return realsize;
}

static int _execute_curl(CURL* curl, logintc_t const* const logintc,
        char const* const path, char const* const body,
        logintc_response_t* response, logintc_error_t* error) {
    CURLcode res;
    long http_code = 0;

    struct curl_slist *headers = NULL;

    char* url;
    char* header_content_type;
    char* header_user_agent;
    char* header_authorization;

    struct logintc_response_body_t response_body;

    int rc = LOGINTC_STATUS_OK;

    response_body.body = malloc(1); /* will be grown as needed by the realloc above */
    response_body.size = 0;

    url = (char*) malloc(strlen(logintc->api_url) + strlen(path) + 1);
    snprintf(url, strlen(logintc->api_url) + strlen(path) + 1, "%s%s",
            logintc->api_url, path);

    if (curl) {

        curl_easy_setopt(curl, CURLOPT_URL, url);

        if (body != NULL) {
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body);

            header_content_type = (char*) malloc(
                    14 + strlen(LOGINTC_CONTENT_TYPE) + 1);
            snprintf(header_content_type,
                    14 + strlen(LOGINTC_CONTENT_TYPE) + 1, "Content-Type: %s",
                    LOGINTC_CONTENT_TYPE);

            headers = curl_slist_append(headers, header_content_type);
        }

        header_user_agent
                = (char*) malloc(12 + strlen(logintc->user_agent) + 1);
        snprintf(header_user_agent, 12 + strlen(logintc->user_agent) + 1,
                "User-Agent: %s", logintc->user_agent);

        header_authorization
                = (char*) malloc(29 + strlen(logintc->api_key) + 1);
        snprintf(header_authorization, 29 + strlen(logintc->api_key) + 1,
                "Authorization: LoginTC key=\"%s\"", logintc->api_key);

        headers = curl_slist_append(headers, header_user_agent);
        headers = curl_slist_append(headers, header_authorization);

        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION,
                _write_response_body_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *) &response_body);

        /* Perform the request, res will get the return code */
        res = curl_easy_perform(curl);
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);

        /* Check for errors */
        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                    curl_easy_strerror(res));

            rc = LOGINTC_STATUS_ERROR;

            if (error != NULL) {

                error->error_type = LOGINTC_CLIENT_CURL_ERR;
                error->error_code = res;
                error->error_message = (char*) malloc(
                        strlen(curl_easy_strerror(res)) + 1);
                snprintf(error->error_message,
                        strlen(curl_easy_strerror(res)) + 1, "%s",
                        curl_easy_strerror(res));
            } else {
                fprintf(stderr, "NULL logintc_error_t passed.\n");
                exit(1);
            }
        } else if (http_code != 200 && http_code != 201) {
            fprintf(stderr, "API failed: %ld\n", http_code);

            rc = LOGINTC_STATUS_ERROR;

            if (error != NULL) {
                error->error_type = LOGINTC_CLIENT_API_ERR;
                error->error_code = http_code;

                if (response_body.body) {
                    fprintf(stderr, "API failed: %s\n", response_body.body);

                    error->error_message = (char*) malloc(
                            strlen(response_body.body) + 1);
                    snprintf(error->error_message,
                            strlen(response_body.body) + 1, "%s",
                            response_body.body);
                }
            } else {
                fprintf(stderr, "NULL logintc_error_t passed.\n");
                exit(1);
            }
        } else {
            if (response_body.body) {
                response->body = (char*) malloc(strlen(response_body.body) + 1);

                snprintf(response->body, strlen(response_body.body) + 1, "%s",
                        response_body.body);
            }

            curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE,
                    (long*) &(response->http_code));
        }

        /* always cleanup */
        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);

    }

    free(response_body.body);

    return rc;

}

static int _logintc_client_get(logintc_t const* const logintc,
        char const* const path, logintc_response_t* response,
        logintc_error_t* error) {

    CURL * curl;

    curl = curl_easy_init();

    curl_easy_setopt(curl, CURLOPT_HTTPGET, 1);

    return _execute_curl(curl, logintc, path, NULL, response, NULL);
}

static int _logintc_client_post(logintc_t const* const logintc,
        char const* const path, char const* const body,
        logintc_response_t* response, logintc_error_t* error) {

    CURL * curl;

    curl = curl_easy_init();

    curl_easy_setopt(curl, CURLOPT_POST, 1);

    return _execute_curl(curl, logintc, path, body, response, error);
}

logintc_t*
logintc_logintc_new(char const* const api_key) {
    logintc_t* logintc;
    logintc = (logintc_t*) malloc(sizeof(logintc_t));
    logintc->api_url = (char*) malloc(strlen(LOGINTC_API_URL) + 1);
    logintc->api_key = (char*) malloc(strlen(api_key) + 1);
    logintc->user_agent = (char*) malloc(
            strlen(LOGINTC_NAME) + 1 + strlen(LOGINTC_VERSION) + 1);

    snprintf(logintc->api_url, strlen(LOGINTC_API_URL) + 1, "%s",
            LOGINTC_API_URL);
    snprintf(logintc->api_key, strlen(api_key) + 1, "%s", api_key);
    snprintf(logintc->user_agent,
            strlen(LOGINTC_NAME) + 1 + strlen(LOGINTC_VERSION) + 1, "%s/%s",
            LOGINTC_NAME, LOGINTC_VERSION);

    return logintc;
}

void logintc_logintc_free(logintc_t* logintc) {
    if (logintc) {
        free(logintc->api_key);
        free(logintc->api_url);
        free(logintc);
    }

    logintc = NULL;
}

logintc_session_t* logintc_create_session_with_username(
        logintc_t const* const logintc, char const* const domain_id,
        char const* const user_name, logintc_error_t* error) {
    CURL* curl;
    char* escaped_domain_id;
    char* escaped_user_name;

    char* path;
    char* body;

    logintc_response_t* response;

    logintc_session_t* session = NULL;
    int rc;

    curl = curl_easy_init();
    escaped_domain_id = curl_easy_escape(curl, domain_id, 0);
    escaped_user_name = curl_easy_escape(curl, user_name, 0);
    curl_easy_cleanup(curl);

    path = (char*) malloc(22 + strlen(escaped_domain_id) + 1);
    snprintf(path, 22 + strlen(escaped_domain_id) + 1,
            "/api/domains/%s/sessions", escaped_domain_id);

    body = (char*) malloc(41 + strlen(escaped_user_name) + 1);
    snprintf(body, 41 + strlen(escaped_user_name) + 1,
            "{\"user\":{\"username\":\"%s\"}, \"attributes\":[]}",
            escaped_user_name);

    response = logintc_response_new();

    rc = _logintc_client_post(logintc, path, body, response, error);

    if (rc == LOGINTC_STATUS_OK) {
        session = logintc_session_new(response->body);
    }

    curl_free(escaped_domain_id);
    curl_free(escaped_user_name);
    free(body);
    free(path);
    logintc_response_free(response);

    return session;
}

void logintc_get_session(logintc_t const* const logintc,
        char const* const domain_id, logintc_session_t** session) {
    CURL* curl;
    char* escaped_domain_id;
    logintc_response_t* response;
    logintc_error_t* error = NULL;
    char* path;
    int rc;
    logintc_session_t* tmp;

    curl = curl_easy_init();
    escaped_domain_id = curl_easy_escape(curl, domain_id, 0);
    curl_easy_cleanup(curl);

    path = (char*) malloc(
            23 + strlen(escaped_domain_id) + strlen((*session)->id) + 1);
    snprintf(path, 23 + strlen(escaped_domain_id) + strlen((*session)->id) + 1,
            "/api/domains/%s/sessions/%s", escaped_domain_id, (*session)->id);

    response = logintc_response_new();

    rc = _logintc_client_get(logintc, path, response, error);

    if (rc == LOGINTC_STATUS_OK) {
        tmp = *session;
        *session = logintc_session_new(response->body);
        free(tmp);
    }

    curl_free(escaped_domain_id);
    free(path);
    logintc_response_free(response);

}
