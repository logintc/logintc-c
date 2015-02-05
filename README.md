Introduction
============

The LoginTC C client is a LoginTC [REST API][rest-api] client to
authenticate using LoginTC.

This package allows a developer to quickly add multi-factor authentication to your existing C applications.

Installation
============

(tested on CentOS 6.4)

Dependencies:

    sudo yum install git gcc gcc-c++ make curl-devel cmake
    
Get and build the code:

    $ git clone https://github.com/logintc/logintc-c.git
    $ pushd logintc-c
    $ cmake .
    $ make
    $ popd

When installed, program headers to be copied to /usr/local/include and shared and static library objects to /usr/local/lib. You can change the installation location by adding the --prefix flag:

    $ cmake -DCMAKE_INSTALL_PREFIX:PATH=/opt/logintc-c .

You can pass compiler and linker flags by setting the CFLAGS and LDFLAGS variables. This is useful if you want to build a binary with debugging flags, more strict options, or optimization options:

    $ CFLAGS="-g -ansi -pedantic -Wall" cmake .

Install the binaries

    $ pushd logintc-c
    $ sudo make install
    $ popd

The following files are copied:

    include/logintc/*.h (header files)
    lib/liblogintc.so (shared library)

You can also bypass the CMake process and compile the object manually:

    $ pushd logintc-c
    $ gcc -Isrc -lcurl -fPIC -shared -o liblogintc.so src/logintc/*.c
    $ popd

Example
=======

The following example will create an authentication session and wait 60 seconds
for the user to approve or deny:

```c
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

    session = logintc_create_session_with_username(logintc, domain_id, user_name, error);

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

```

This program can be compiled with:

    $ gcc -llogintc -o authentication authentication.c
    
Once the binary is produce run it with:

    $ LD_LIBRARY_PATH=/usr/local/lib ./authentication
    
You may also to set the LD_LIBRARY_PATH explicitly before hand:

    $ export LD_LIBRARY_PATH=/usr/local/lib
    $ ./authentication

Documentation
=============

See <https://www.logintc.com/docs>

Help
====

Email: <support@cyphercor.com>

<https://www.logintc.com>

[rest-api]: https://www.logintc.com/docs/rest-api
