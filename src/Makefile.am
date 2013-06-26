lib_LTLIBRARIES = liblogintc.la

liblogintc_includedir = $(includedir)/logintc

liblogintc_include_HEADERS = logintc/error.h logintc/globals.h logintc/logintc.h logintc/response.h logintc/session.h

liblogintc_la_CFLAGS = -I.
liblogintc_la_LDFLAGS = -no-undefined -lcurl
liblogintc_la_SOURCES = logintc/error.c logintc/logintc.c logintc/response.c logintc/session.c