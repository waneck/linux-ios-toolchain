bin_PROGRAMS = ldid
ldid_SOURCES = \
	       ldid.cpp	\
	       lookup2.c	\
	       sha1.c

ldid_CPPFLAGS = -I$(top_srcdir)/libhelper
ldid_LDADD = $(top_builddir)/libhelper/libhelper.la

EXTRA_DIST =	\
	sha1.h			\
	minimal/hexdump.h	\
	minimal/mapping.h	\
	minimal/sqlite3.h	\
	minimal/stdlib.h	\
	minimal/string.h
