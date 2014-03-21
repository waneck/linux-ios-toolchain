include configure.mk

PREFIX=/usr

# Assembler and linker
cc-tools:
	cd original/cctools-839-ld64-134.9 && ./autogen.sh; ./configure --target=${TARGET} --prefix=${PREFIX} && make

.PHONY: cc-tools
