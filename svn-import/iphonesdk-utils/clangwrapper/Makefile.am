bin_PROGRAMS = ios-clang-wrapper 

ios_clang_wrapper_SOURCES = clangwrapper.cpp

ios_clang_wrapper_CPPFLAGS = 				\
	-I$(top_srcdir)/libhelper			\
	$(XML_CFLAGS)

ios_clang_wrapper_LDADD =				\
	$(top_builddir)/libhelper/libhelper.la \
	$(top_builddir)/libplutil/libplutil.la

install-data-hook:
	ln -sf ios-clang-wrapper $(DESTDIR)/$(bindir)/ios-clang
	ln -sf ios-clang-wrapper $(DESTDIR)/$(bindir)/ios-clang++
	ln -sf ios-clang-wrapper $(DESTDIR)/$(bindir)/ios-switchsdk
