bin_PROGRAMS = ios-xcbuild

ios_xcbuild_SOURCES = xcbuild.cpp \
	libxcodeutils/lex.yy.c \
	libxcodeutils/pbxproj.cpp \
	libxcodeutils/pbxprojdef.cpp

AM_CPPFLAGS = -fPIC -I$(srcdir)/xcbuild/libxcodeutils

EXTRA_DIST =    \
	libxcodeutils/pbxprojdef.h \
	libxcodeutils/pbxproj.hpp \
	libxcodeutils/pbxproj.l \
  libxcodeutils/pbxproj.y \
  libxcodeutils/README \
	xcdump.cpp
