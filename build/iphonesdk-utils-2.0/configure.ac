#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.68])
AC_INIT(iphonesdk-utils, 2.0, [cjacker@gmail.com])

AC_CONFIG_HEADERS(config.h)
AC_CONFIG_SRCDIR(clangwrapper/clangwrapper.cpp)

AM_INIT_AUTOMAKE([1.10 no-define no-dist-gzip dist-bzip2 tar-ustar])
AC_CONFIG_MACRO_DIR([m4])
AM_MAINTAINER_MODE

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AM_PROG_CC_C_O
# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([stddef.h stdint.h stdlib.h])

LT_PREREQ([2.2.6])
LT_INIT(disable-static)

# Checks for typedefs, structures, and compiler characteristics.
#AC_CHECK_HEADER_STDBOOL
AC_TYPE_INT16_T
AC_TYPE_INT32_T
AC_TYPE_PID_T
AC_TYPE_SIZE_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT8_T

# Checks for library functions.
AC_FUNC_CHOWN
AC_FUNC_FORK
AC_CHECK_FUNCS([memset strdup strrchr strtoul])
AC_CHECK_LIB([z],[crc32],[
         ZLIB_LIBS=-lz
         ])
AC_SUBST(ZLIB_LIBS)

AC_CHECK_LIB([png],[png_create_read_struct],[
         PNG_LIBS=-lpng
         ])
AC_SUBST(PNG_LIBS)

AC_CHECK_LIB([m],[pow],[
         M_LIBS=-lm
         ])
AC_SUBST(M_LIBS)


PKG_CHECK_MODULES(XML, [
    libxml-2.0 
])

AC_CHECK_PROG(HAVE_LLVM_CONFIG, llvm-config, true, false)

if test "x$HAVE_LLVM_CONFIG" = "xfalse"; then
	AC_CHECK_PROG(HAVE_LLVM_CONFIG_32, llvm-config-3.2, true, false)
	if test "x$HAVE_LLVM_CONFIG_32" = "xfalse"; then
		AC_MSG_ERROR([llvm-config or llvm-config-3.2 not found on your system,please install related packages])
	else
    		LLVM_CXX_FLAGS=`llvm-config-3.2 --cxxflags`
    		LLVM_LIBDIR=`llvm-config-3.2 --libdir`
    		LLVM_LDFLAGS=`llvm-config-3.2 --ldflags --libs cppbackend|sed 'N;s/\n//'`
    		LLVM_LIBS=`llvm-config-3.2 --libs`
    		LLVM_LIBS="-L${LLVM_LIBDIR} ${LLVM_LIBS} ${LLVM_LDFLAGS}"

    		AC_SUBST(LLVM_CXX_FLAGS)
    		AC_SUBST(LLVM_LIBS)
	fi
else
    LLVM_CXX_FLAGS=`llvm-config --cxxflags`
    LLVM_LIBDIR=`llvm-config --libdir`
    LLVM_LDFLAGS=`llvm-config --ldflags --libs cppbackend|sed 'N;s/\n//'`
    LLVM_LIBS=`llvm-config --libs`
    LLVM_LIBS="-L${LLVM_LIBDIR} ${LLVM_LIBS} ${LLVM_LDFLAGS}"

    AC_SUBST(LLVM_CXX_FLAGS)
    AC_SUBST(LLVM_LIBS)
fi

TEMPLATES_DIR="$datadir/iPhoneTemplates"
AC_SUBST(TEMPLATES_DIR)

AC_OUTPUT([
	   Makefile
	   libhelper/Makefile
	   libplutil/Makefile
	   ldid/Makefile
	   clangwrapper/Makefile
	   plutil/Makefile
	   createProject/Makefile
	   createProject/templates/Makefile
	   genLocalization2/Makefile
	   pngcrush/Makefile
	   xcbuild/Makefile
])
