include configure.mk

PREFIX=/usr

CCTOOLS=build/cctools/cctools
IOSTOOLS=build/iphonesdk-utils-2.0

no-configure:
	echo "Please run ./configure before running make"

# Assembler and linker
cctools.stamp:
	cd ${CCTOOLS} && \
		./autogen.sh; \
		./configure --target=${TARGET} --prefix=${PREFIX} && \
		make
	touch $@

cctools-install.stamp: cctools.stamp
	make install -C ${CCTOOLS}
	touch $@

cctools-clean:
	make clean -C ${CCTOOLS}
	rm -f cctools.stamp

# Utilities
ios-tools.stamp:
	cd ${IOSTOOLS} && \
			./autogen.sh ; \
			./configure --prefix=${PREFIX} && \
			make
	touch $@

ios-tools-install.stamp: ios-tools.stamp
	make install -C ${IOSTOOLS}
	touch $@

ios-tools-clean:
	make clean -C ${IOSTOOLS}
	rm -f ios-tools.stamp


clean: cctools-clean ios-tools-clean

.PHONY: clean ios-tools-clean cctools-clean
