include configure.mk

PREFIX=/usr
LLVM_INCLUDE=/usr/include/llvm-c-3.2

CCTOOLS=original/cctools-839-ld64-134.9

# Assembler and linker
cctools:
	cd ${CCTOOLS} && \
		./autogen.sh; \
		CFLAGS="-I${LLVM_INCLUDE}" CXXFLAGS="-I${LLVM_INCLUDE}" ./configure --target=${TARGET} --prefix=${PREFIX} && \
		make

cctools-clean:
	make clean -C ${CCTOOLS}

.PHONY: cctools cctools-clean
