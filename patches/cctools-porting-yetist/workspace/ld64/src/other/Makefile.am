bin_PROGRAMS =  \
	dyldinfo \
	ObjectDump \
	unwinddump \
	machochecker 

AM_CXXFLAGS = \
	-D__DARWIN_UNIX03 \
	$(WARNINGS) \
	$(LTO_DEFS) \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/include/foreign \
	-I$(top_srcdir)/ld64/src \
	-I$(top_srcdir)/ld64/src/3rd \
	-I$(top_srcdir)/ld64/src/abstraction \
	-I$(top_srcdir)/ld64/src/ld \
	-I$(top_srcdir)/ld64/src/ld/parsers \
	-I$(top_srcdir)/ld64/src/ld/passes

AM_CFLAGS = \
    -D__DARWIN_UNIX03 \
    $(WARNINGS) \
    $(LTO_DEFS) \
    -I$(top_srcdir)/include \
    -I$(top_srcdir)/include/foreign \
    -I$(top_srcdir)/ld64/src \
    -I$(top_srcdir)/ld64/src/3rd \
    -I$(top_srcdir)/ld64/src/abstraction \
    -I$(top_srcdir)/ld64/src/ld \
    -I$(top_srcdir)/ld64/src/ld/parsers \
    -I$(top_srcdir)/ld64/src/ld/passes 

unwinddump_SOURCES = unwinddump.cpp
machochecker_SOURCES = machochecker.cpp

if CC_IS_CLANG
ObjectDump_CFLAGS = -fblocks $(AM_CFLAGS)
endif
if CXX_IS_CLANGPLUSPLUS
ObjectDump_CXXFLAGS = -fblocks $(AM_CXXFLAGS)
endif
ObjectDump_SOURCES = \
	ObjectDump.cpp \
	$(top_srcdir)/ld64/src/ld/debugline.c 
ObjectDump_LDADD =  \
	$(top_builddir)/ld64/src/ld/parsers/libParsers.la \
	$(LTO_LIBS)  


dyldinfo_SOURCES =  dyldinfo.cpp
dyldinfo_LDADD =  \
	$(top_builddir)/ld64/src/3rd/libhelper.la


