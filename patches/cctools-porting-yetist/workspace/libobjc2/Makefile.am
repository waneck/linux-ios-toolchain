noinst_LTLIBRARIES = libobjc.la

AM_LIBTOOLFLAGS = --tag=CC
libobjc_la_CFLAGS = -std=gnu99 -fPIC -fexceptions -Wno-deprecated-objc-isa-usage -DTYPE_DEPENDENT_DISPATCH -DGNUSTEP -D__OBJC_RUNTIME_INTERNAL__=1 -D_XOPEN_SOURCE=500 -D__BSD_VISIBLE=1 -D_BSD_SOURCE=1 -DNO_SELECTOR_MISMATCH_WARNINGS

#if CC_IS_GCC
#AM_CPPFLAGS =-std=gnu99
#endif
#if OBJC_IS_GCC
#AM_CPPFLAGS =-std=gnu99
#endif
libobjc_la_SOURCES =  \
    NSBlocks.m	\
    Protocol2.m	\
    abi_version.c	\
    alias_table.c	\
    arc.m	\
    associate.m	\
    blocks_runtime.m	\
    block_to_imp.c	\
    caps.c	\
    category_loader.c	\
    class_table.c	\
    dtable.c	\
    eh_personality.c	\
    encoding2.c	\
    gc_none.c	\
    hash_table.c	\
    hooks.c	\
    ivar.c	\
    legacy_malloc.c	\
    loader.c	\
    mutation.m	\
    properties.m	\
    protocol.c	\
    runtime.c	\
    sarray2.c	\
    selector_table.c	\
    sendmsg2.c	\
    statics_loader.c	\
    toydispatch.c	\
    block_trampolines.S	\
    objc_msgSend.S

EXTRA_DIST =	\
    	objc_msgSend.arm.S	\
	objc_msgSend.x86-32.S	\
	objc_msgSend.x86-64.S	\
	alias.h	\
	blocks_runtime.h	\
	buffer.h	\
	category.h	\
	class.h	\
	constant_string.h	\
	dtable.h	\
	dwarf_eh.h	\
	gc_ops.h	\
	hash_table.h	\
	ivar.h	\
	loader.h	\
	lock.h	\
	method_list.h	\
	module.h	\
	nsobject.h	\
	objcxx_eh.h	\
	pool.h	\
	properties.h	\
	protocol.h	\
	sarray2.h	\
	selector.h	\
	slot_pool.h	\
	spinlock.h	\
	string_hash.h	\
	type_encoding_cases.h	\
	unistd.h	\
	unwind-arm.h	\
	unwind.h	\
	unwind-itanium.h	\
	visibility.h	\
	objc	\
	opts
