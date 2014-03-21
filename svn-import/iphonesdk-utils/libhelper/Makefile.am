#AM_CPPFLAGS = $(XML_CFLAGS)

noinst_LTLIBRARIES = libhelper.la
noinst_HEADERS = helper.h

libhelper_la_LIBADD = $(XML_LIBS) $(top_builddir)/libplutil/libplutil.la
libhelper_la_CPPFLAGS = $(XML_CFLAGS)  -I$(top_srcdir)/libplutil

libhelper_la_SOURCES = 	\
	xmlutil.cpp	\
	stringutil.cpp	\
	pathutil.cpp	\
	sdkutil.cpp
