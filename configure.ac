
# Prelude
AC_INIT([GOpenCL], [0.1], [juanrgar@gmail.com])
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_MACRO_DIR([m4])
AC_CANONICAL_HOST
AM_INIT_AUTOMAKE([-Wall -Werror])
AM_PROG_AR

# Checks for programs.
AC_PROG_CC
AC_PROG_CXX
PKG_PROG_PKG_CONFIG
LT_INIT

# Checks for libraries.
AC_MSG_WARN([Only Apple's implementation has been tested.])

# Checks for header files.
case "${host_os}" in
*darwin* | *rhapsody* | *macosx*)
    AC_MSG_WARN([Platform is APPLE.])
    AC_CHECK_HEADERS([OpenCL/opencl.h])
    LIBS="$LIBS -framework OpenCL"
    ;;
*)
    AC_MSG_WARN([Platform is OTHER/POSIX.])
    AC_CHECK_HEADERS([CL/opencl.h])
    LIBS="$LIBS -lOpenCL"
    ;;
esac


PKG_CHECK_MODULES([GLIB], [glib-2.0])
AC_SUBST([GLIB_CFLAGS])
AC_SUBST([GLIB_LIBS])

PKG_CHECK_MODULES([GOBJECT], [gobject-2.0])
AC_SUBST([GOBJECT_CFLAGS])
AC_SUBST([GOBJECT_LIBS])

GOBJECT_INTROSPECTION_CHECK

GTK_DOC_CHECK([1.14], [--flavour no-tmpl])

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.

# Output files.
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIR(m4)

AC_CONFIG_FILES([Makefile src/Makefile examples/Makefile doc/Makefile doc/reference/Makefile doc/reference/gopencl/Makefile])
AC_OUTPUT

echo ""
echo "                  GOpencl $VERSION"
echo "                  ================"
echo "           Install prefix:  ${prefix}"
echo " Build introspection data:  ${enable_introspection}"
echo "  Build API documentation:  ${enable_gtk_doc}"
echo "        Enable debug mode:  ${enable_debug}"
echo ""

