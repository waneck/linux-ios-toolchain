AM_CPPFLAGS =	\
	-I$(srcdir)/libpng	\
	-DPNG_CONFIGURE_LIBPNG	\
	-DPNG_NO_PROGRESSIVE_READ	\
	-DPNG_NO_EASY_ACCESS

bin_PROGRAMS = ios-pngcrush 

ios_pngcrush_SOURCES = main.c crush.c decrush.c \
	libpng/png.c \
	libpng/pngerror.c \
	libpng/pngget.c \
	libpng/pngmem.c \
	libpng/pngpread.c \
	libpng/pngread.c \
	libpng/pngrio.c \
	libpng/pngrtran.c \
	libpng/pngrutil.c \
	libpng/pngset.c \
	libpng/pngtrans.c \
	libpng/pngwio.c \
	libpng/pngwrite.c \
	libpng/pngwtran.c \
	libpng/pngwutil.c

ios_pngcrush_LDADD = $(ZLIB_LIBS) $(M_LIBS)

EXTRA_DIST =    \
    libpng/pngconf.h    \
    libpng/png.h    \
    libpng/pngpriv.h
