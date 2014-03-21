include configure.mk

PREFIX=/usr
LLVM_INCLUDE=/usr/include/llvm-c-3.2

CCTOOLS=build/cctools-839-ld64-134.9
IOSTOOLS=build/iphonesdk-utils-2.0

no-configure:
	echo "Please run ./configure before running make"

# Assembler and linker
cctools:
	cd ${CCTOOLS} && \
		./autogen.sh; \
		CFLAGS="-I${LLVM_INCLUDE}" CXXFLAGS="-I${LLVM_INCLUDE}" ./configure --target=${TARGET} --prefix=${PREFIX} && \
		make

cctools-install: cctools
	make install -C ${CCTOOLS}

cctools-clean:
	make clean -C ${CCTOOLS}

# Utilities
ios-tools:
	cd ${IOSTOOLS} && \
		./autogen.sh ; \
		./configure --prefix=${PREFIX} && \
		make

ios-tools-install: ios-tools
	make install -C ${IOSTOOLS}

ios-tools-clean:
	make clean -C ${IOSTOOLS}


clean: cctools-clean ios-tools-clean

.PHONY: cctools cctools-clean cctools-install all install no-configure ios-tools ios-tools-install ios-tools-clean clean
