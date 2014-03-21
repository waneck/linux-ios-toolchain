include configure.mk

PREFIX=/usr
LLVM_INCLUDE=/usr/include/llvm-c-3.2

CCTOOLS=original/cctools-839-ld64-134.9

all: cctools

install: cctools-install

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


.PHONY: cctools cctools-clean cctools-install all install
