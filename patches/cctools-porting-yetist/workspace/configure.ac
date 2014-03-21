AC_INIT([ios-toolchain], [1.1], [cjacker@gmail.com])

AC_CANONICAL_BUILD
AC_CANONICAL_HOST
AC_CANONICAL_TARGET

AM_INIT_AUTOMAKE([1.10 no-define no-dist-gzip dist-bzip2 tar-ustar])
AC_CONFIG_HEADER([config.h])
AC_CONFIG_MACRO_DIR([m4])
AM_MAINTAINER_MODE

AC_PREREQ([2.57])
AC_REVISION([$Revision: 1.37 $])

AC_CONFIG_SRCDIR([ar/ar.c])
LT_PREREQ([2.2.6])
LT_INIT(disable-static)

case $target_cpu in
     powerpc)
        AC_DEFINE(EMULATED_HOST_CPU_TYPE, 18, [Emulated CPU type])
        AC_DEFINE(EMULATED_HOST_CPU_SUBTYPE, 100, [Emulated CPU subtype])
    ;;
     powerpc64)
        AC_DEFINE(EMULATED_HOST_CPU_TYPE, 16777234, [Emulated CPU type])
        AC_DEFINE(EMULATED_HOST_CPU_SUBTYPE, 0, [Emulated CPU subtype])
       ;;
     i?86)
        AC_DEFINE(EMULATED_HOST_CPU_TYPE, 7, [Emulated CPU type])
        AC_DEFINE(EMULATED_HOST_CPU_SUBTYPE, 10, [Emulated CPU subtype])
    ;;
     x86_64)
        AC_DEFINE(EMULATED_HOST_CPU_TYPE, 16777223, [Emulated CPU type])
        AC_DEFINE(EMULATED_HOST_CPU_SUBTYPE, 3, [Emulated CPU subtype])
    ;;
     arm)
        AC_DEFINE(EMULATED_HOST_CPU_TYPE, 12, [Emulated CPU type])
        AC_DEFINE(EMULATED_HOST_CPU_SUBTYPE, 0, [Emulated CPU subtype])
    ;;
     *)
    AC_MSG_ERROR([Unsupported target $target_cpu])
    ;;
esac

#force compiler to clang
CC=clang
CCAS=gcc
AC_PROG_CC([clang])
AM_PROG_AS
AC_PROG_CXX([clang++])
AM_PROG_CC_C_O
AC_PROG_OBJC([clang])
AC_PROG_OBJCXX([clang++])

AC_PROG_INSTALL
AC_PROG_LN_S
AC_ARG_PROGRAM

#as reported, some users may use llvm-c/Disassembler.h from base system instead of the local copy in include dir.
#add these macros for avoid compilation failed.

AC_DEFINE(__STDC_LIMIT_MACROS, [1], [Limits of specified-width integer types])
AC_DEFINE(__STDC_CONSTANT_MACROS, [1], [Define Macros for integer constants])

AC_CHECK_TOOL([HOST_RANLIB], [ranlib], :)
AC_CHECK_TOOL([HOST_AR], [ar], :)

AC_SUBST([ASLIBEXECDIR], ['${libexecdir}/as'])

if test "$target_alias"; then
AC_SUBST([PROGRAM_PREFIX], ['${target_alias}-'])
fi
rm -f conftest.$ac_objext conftest$ac_exeext conftest.$ac_ext

WARNINGS=""

ORIGCFLAGS=$CFLAGS
CFLAGS="$CFLAGS -Wall"
AC_MSG_CHECKING([if -Wall is supported])
AC_COMPILE_IFELSE(
   [AC_LANG_SOURCE([[const char hw[] = "Hello, World\n";]])],
   [WARNINGS="$WARNINGS -Wall"
    AC_MSG_RESULT([yes])],
   [AC_MSG_RESULT([no])]
)
CFLAGS=$ORIGCFLAGS

ORIGCFLAGS=$CFLAGS
CFLAGS="$CFLAGS -Wno-long-long"
AC_MSG_CHECKING([if -Wno-long-long is supported])
AC_COMPILE_IFELSE(
   [AC_LANG_SOURCE([[const char hw[] = "Hello, World\n";]])],
   [WARNINGS="$WARNINGS -Wno-long-long"
    AC_MSG_RESULT([yes])],
   [AC_MSG_RESULT([no])]
)
CFLAGS=$ORIGCFLAGS

ORIGCFLAGS=$CFLAGS
CFLAGS="$CFLAGS -Wno-import"
AC_MSG_CHECKING([if -Wno-import is supported])
AC_COMPILE_IFELSE(
   [AC_LANG_SOURCE([[const char hw[] = "Hello, World\n";]])],
   [WARNINGS="$WARNINGS -Wno-import"
    AC_MSG_RESULT([yes])],
   [AC_MSG_RESULT([no])]
)
CFLAGS=$ORIGCFLAGS

ORIGCFLAGS=$CFLAGS
CFLAGS="$CFLAGS -Wno-format"
AC_MSG_CHECKING([if -Wno-format is supported])
AC_COMPILE_IFELSE(
   [AC_LANG_SOURCE([[const char hw[] = "Hello, World\n";]])],
   [WARNINGS="$WARNINGS -Wno-format"
    AC_MSG_RESULT([yes])],
   [AC_MSG_RESULT([no])]
)
CFLAGS=$ORIGCFLAGS

ORIGCFLAGS=$CFLAGS
CFLAGS="$CFLAGS -Wno-deprecated"
AC_MSG_CHECKING([if -Wno-deprecated is supported])
AC_COMPILE_IFELSE(
   [AC_LANG_SOURCE([[const char hw[] = "Hello, World\n";]])],
   [WARNINGS="$WARNINGS -Wno-deprecated"
    AC_MSG_RESULT([yes])],
   [AC_MSG_RESULT([no])]
)
CFLAGS=$ORIGCFLAGS

ORIGCFLAGS=$CFLAGS
CFLAGS="$CFLAGS -Wno-unused-variable"
AC_MSG_CHECKING([if -Wno-unused-variable is supported])
AC_COMPILE_IFELSE(
   [AC_LANG_SOURCE([[const char hw[] = "Hello, World\n";]])],
   [WARNINGS="$WARNINGS -Wno-unused-variable"
    AC_MSG_RESULT([yes])],
   [AC_MSG_RESULT([no])]
)
CFLAGS=$ORIGCFLAGS

#ORIGCFLAGS=$CFLAGS
CFLAGS="$CFLAGS -Wno-invalid-offsetof"
AC_MSG_CHECKING([if -Wno-invalid-offsetof is supported])
AC_COMPILE_IFELSE(
   [AC_LANG_SOURCE([[const char hw[] = "Hello, World\n";]])],
   [WARNINGS="$WARNINGS -Wno-invalid-offsetof"
    AC_MSG_RESULT([yes])],
   [AC_MSG_RESULT([no])]
)
CFLAGS=$ORIGCFLAGS

AC_SUBST([WARNINGS], [$WARNINGS])

AC_CHECK_LIB([dl],[dlopen],[
         DL_LIBS=-ldl
         ])
AC_SUBST(DL_LIBS)

AC_CHECK_LIB([pthread],[pthread_create],[
         PTHREAD_FLAGS=-pthread
         ])
AC_SUBST(PTHREAD_FLAGS)

AC_CHECK_LIB([uuid],[uuid_generate_random],[
         UUID_LIBS=-luuid
         ])
AC_SUBST(UUID_LIBS)

AC_CHECK_LIB([crypto],[MD5_Init],[
         CRYPT_LIBS=-lcrypto
         ])
AC_SUBST(CRYPT_LIBS)

AC_C_BIGENDIAN([AC_SUBST([ENDIAN_FLAG],[-D__BIG_ENDIAN__])], [AC_SUBST([ENDIAN_FLAG],[-D__LITTLE_ENDIAN__])])
#####################
# check CC and CXX
#####################
if test "x$CC" != "xclang";then
	CFLAGS="$CFLAGS $ENDIAN_FLAG -D__private_extern__=extern"
fi
if test "x$CXX" != "xclang++";then
	CXXFLAGS="$CXXFLAGS $ENDIAN_FLAG -D__private_extern__=extern"
fi
AM_CONDITIONAL(CC_IS_GCC, [test "x$CC" = "xgcc"])
AM_CONDITIONAL(CC_IS_CLANG, [test "x$CC" = "xclang"])
AM_CONDITIONAL(OBJC_IS_GCC, [test "x$OBJC" = "xgcc"])
AM_CONDITIONAL(OBJC_IS_CLANG, [test "x$OBJC" = "xclang"])
AM_CONDITIONAL(CXX_IS_GCC, [test "x$CXX" = "xg++"])
AM_CONDITIONAL(CXX_IS_CLANGPLUSPLUS, [test "x$CXX" = "xclang++"])
AM_CONDITIONAL(OBJCXX_IS_GPLUSPLUS, [test "x$OBJCXX" = "xg++"])
AM_CONDITIONAL(OBJCXX_IS_CLANGPLUSPLUS, [test "x$OBJCXX" = "xclang++"])

#####################
# Check LTO
#####################
AC_CHECK_PROG([llvm_config], [llvm-config],[yes],[no])
if test "x$llvm-config" = "xno"; then
        AC_MSG_ERROR("Could not find llvm-config.")
else
        LLVM_LIBDIR=`llvm-config --libdir`
        if test -f $LLVM_LIBDIR/libLTO.so;then
                LTO_LIBS="-L`llvm-config --libdir` -lLTO"
                LTO_DEFS="-DLTO_SUPPORT=1"
        fi
fi

AC_SUBST(LTO_LIBS)
AC_SUBST(LTO_DEFS)

#####################
# Check LD_CLASSIC
#####################
cat > conftest.c <<EOF
#if defined(__x86_64__) || (!defined(__i386__) && defined(__LP64__))
host_address=64
#else
host_address=32
#endif
EOF
OSBIT=`${CC-cc} -E conftest.c | grep host_address=`
rm -f conftest.c

if test "$OSBIT" = "host_address=32"; then
	LD_CLASSIC="yes"
	AC_MSG_NOTICE(["Your system is 32bit, enable ld_classic build"])
else
	LD_CLASSIC="no"
	AC_MSG_NOTICE(["Your system is 64bit, disable ld_classic"])
fi

AM_CONDITIONAL(ENABLE_LD_CLASSIC, [test "x$LD_CLASSIC" = "xyes"])


AC_CONFIG_FILES([
	Makefile
	libstuff/Makefile
	ar/Makefile
	as/Makefile
	as/arm/Makefile
	as/i386/Makefile
	as/x86_64/Makefile
	as/ppc/Makefile
	as/ppc64/Makefile
	misc/Makefile
	otool/Makefile
	libobjc2/Makefile
	ld/Makefile
	ld64/Makefile
	ld64/src/Makefile
	ld64/src/3rd/Makefile
	ld64/src/3rd/BlocksRuntime/Makefile
	ld64/src/ld/Makefile
	ld64/src/ld/parsers/Makefile
	ld64/src/ld/passes/Makefile
	ld64/src/other/Makefile])

AC_OUTPUT

echo "

Configuration:

	Target:			${target}
	C Compiler:		${CC}
	CCAS Compiler:		${CCAS}
	CXX Compiler:		${CXX}
	OBJC Compiler:		${OBJC}
	OBJCXX Compiler:	${OBJCXX}
	Enable ld_classic:	${LD_CLASSIC}
"
