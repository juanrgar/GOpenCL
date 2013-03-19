
# Prelude
AC_INIT([GOpenCL], [0.1], [juanrgar@gmail.com])
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_MACRO_DIR([m4])
AM_INIT_AUTOMAKE([-Wall -Werror])

# Checks for programs.
AC_PROG_CC
AC_PROG_CXX
LT_INIT

# Checks for libraries.
AC_MSG_WARN([Only Apple's implementation has been tested.])

# Checks for header files.
case "$host_os" in
*darwin* | *rhapsody* | *macosx*)
    AC_CHECK_HEADERS([OpenCL/opencl.h])
    LIBS="$LIBS -framework OpenCL"
    ;;
*)
    AC_CHECK_HEADERS([CL/opencl.h])
    LIBS="$LIBS -lOpenCL"
    ;;
esac

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.

# Output files.
AC_CONFIG_HEADERS([config.h])

AC_CONFIG_FILES([Makefile src/Makefile examples/Makefile])
AC_OUTPUT

