
AM_CPPFLAGS = -I$(top_srcdir)/src/ $(GLIB_CFLAGS) $(GOBJECT_CFLAGS)
LDADD = $(GLIB_LIBS) $(GOBJECT_LIBS)

bin_PROGRAMS = platform-test device-test context-test

platform_test_SOURCES = platform_test.c
platform_test_LDADD = ../src/libGOpenCL.la $(LDADD)

device_test_SOURCES = device_test.c
device_test_LDADD = ../src/libGOpenCL.la $(LDADD)

context_test_SOURCES = context_test.c
context_test_LDADD = ../src/libGOpenCL.la $(LDADD)

