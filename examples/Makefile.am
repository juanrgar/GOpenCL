
AM_CPPFLAGS = -I$(top_srcdir)/src/ $(GLIB_CFLAGS) $(GOBJECT_CFLAGS)
LDADD = $(GLIB_LIBS) $(GOBJECT_LIBS)

bin_PROGRAMS = platform-test

platform_test_SOURCES = platform_test.c
platform_test_LDADD = ../src/libGOpenCL.la $(GLIB_LIBS) $(GOBJECT_LIBS)

