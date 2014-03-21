bin_PROGRAMS = ios-genLocalization

CLANG_LIBS = \
                -lclangTooling\
                -lclangFrontendTool\
                -lclangFrontend\
                -lclangDriver\
                -lclangSerialization\
                -lclangCodeGen\
                -lclangParse\
                -lclangSema\
                -lclangStaticAnalyzerFrontend\
                -lclangStaticAnalyzerCheckers\
                -lclangStaticAnalyzerCore\
                -lclangAnalysis\
                -lclangARCMigrate\
                -lclangEdit\
                -lclangAST\
                -lclangLex\
                -lclangBasic

ios_genLocalization_CPPFLAGS =	\
	$(LLVM_CXX_FLAGS)	\
	-I$(top_srcdir)/libhelper	\
	-I$(top_srcdir)/libplutil	\
	-fno-rtti

ios_genLocalization_LDADD =	\
       	$(top_builddir)/libplutil/libplutil.la	\
      	$(top_builddir)/libhelper/libhelper.la	\
       	$(CLANG_LIBS)	\
	$(LLVM_LIBS)	\
	-ldl

ios_genLocalization_SOURCES =	\
	main.cpp	\
       	getLocalizedStringFromFile.cpp

EXTRA_DIST = getLocalizedStringFromFile.h
