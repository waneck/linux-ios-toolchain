all: libxcodeutils

libxcodeutils:
	lex pbxproj.l
	bison -d pbxproj.y -o pbxproj.cpp
	clang++ -g -c pbx*.cpp -fPIC
	clang -g -c *.c -fPIC
	clang++ -shared -o libxcodeutils.so *.o -fPIC
	ar cr libxcodeutils.a *.o
install:
	mkdir -p $(DESTDIR)/usr/include
	mkdir -p $(DESTDIR)/usr/lib
	install -m 0644 pbxprojdef.h $(DESTDIR)/usr/include/
	install -m 0644 *.so $(DESTDIR)/usr/lib
	install -m 0644 *.a $(DESTDIR)/usr/lib
clean:
	rm -rf lex.yy.c pbxproj.cpp pbxproj.hpp
	rm -rf *.o
	rm -rf libxcodeutils.so
	rm -rf libxcodeutils.a
