SUBDIRS = parsers passes

bin_PROGRAMS = ld
ld_LDADD =  \
	$(top_builddir)/ld64/src/3rd/libhelper.la  \
	$(top_builddir)/ld64/src/3rd/BlocksRuntime/libBlocksRuntime.la  \
	$(top_builddir)/ld64/src/ld/parsers/libParsers.la \
	$(top_builddir)/ld64/src/ld/passes/libPasses.la \
	$(DL_LIBS) \
	$(CRYPT_LIBS) \
	$(UUID_LIBS) \
	$(LTO_LIBS)

ld_LDFLAGS = $(PTHREAD_FLAGS)
ld_CXXFLAGS =  \
	-D__DARWIN_UNIX03 \
	$(WARNINGS) \
	$(LTO_DEFS) \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/include/foreign \
	-I$(top_srcdir)/ld64/src \
	-I$(top_srcdir)/ld64/src/abstraction \
	-I$(top_srcdir)/ld64/src/3rd \
	-I$(top_srcdir)/ld64/src/3rd/BlocksRuntime \
	-I$(top_srcdir)/ld64/src/ld \
	-I$(top_srcdir)/ld64/src/ld/parsers \
	-I$(top_srcdir)/ld64/src/ld/passes \
	-DPROGRAM_PREFIX=\""$(PROGRAM_PREFIX)"\" 

ld_CFLAGS =  \
	-D__DARWIN_UNIX03 \
	$(WARNINGS) \
	$(LTO_DEFS) \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/include/foreign \
	-I$(top_srcdir)/ld64/src \
	-I$(top_srcdir)/ld64/src/abstraction \
	-I$(top_srcdir)/ld64/src/3rd \
	-I$(top_srcdir)/ld64/src/ld \
	-I$(top_srcdir)/ld64/src/ld/parsers \
	-I$(top_srcdir)/ld64/src/ld/passes \
	-DPROGRAM_PREFIX=\""$(PROGRAM_PREFIX)"\" 

if CC_IS_CLANG
ld_CFLAGS += -fblocks
endif
if CXX_IS_CLANGPLUSPLUS
ld_CXXFLAGS += -fblocks
endif

ld_SOURCES =  \
	debugline.c  \
	InputFiles.cpp  \
	ld.cpp  \
	Options.cpp  \
	OutputFile.cpp  \
	Resolver.cpp  \
	Snapshot.cpp  \
	SymbolTable.cpp \
	code-sign-blobs/blob.cpp

EXTRA_DIST =	\
	Architectures.hpp	\
	debugline.h	\
	dwarf2.h	\
	HeaderAndLoadCommands.hpp	\
	InputFiles.h	\
	ld.hpp	\
	LinkEditClassic.hpp	\
	LinkEdit.hpp	\
	lto_file.hpp	\
	Options.h	\
	OutputFile.h	\
	Resolver.h	\
	Snapshot.h	\
	SymbolTable.h	\
	code-sign-blobs
