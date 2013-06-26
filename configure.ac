AC_INIT([logintc-c], [0.1.0], [support@cyphercor.com])

# Init automake
AM_INIT_AUTOMAKE([foreign])

LT_INIT

AC_PROG_CC
AC_CHECK_HEADERS([curl/curl.h openssl/bio.h openssl/buffer.h openssl/evp.h openssl/hmac.h openssl/sha.h stddef.h stdlib.h string.h time.h])
AC_FUNC_MALLOC

AC_CONFIG_FILES([Makefile src/Makefile])
AC_CONFIG_MACRO_DIR([m4])

AC_OUTPUT
