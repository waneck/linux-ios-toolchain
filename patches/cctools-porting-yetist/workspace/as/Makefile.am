SUBDIRS=arm i386 x86_64 ppc ppc64

bin_PROGRAMS = as 
as_LDADD =  \
        $(top_builddir)/libstuff/libstuff.la

as_CFLAGS = -I$(top_srcdir)/as -I$(top_srcdir)/include -I$(top_srcdir)/include/foreign -I$(top_srcdir)/libstuff $(WARNINGS) $(LTO_DEFS) -DNeXT_MOD -DASLIBEXECDIR="\"$(ASLIBEXECDIR)/\"" -D__DARWIN_UNIX03

as_SOURCES = driver.c 

EXTRA_DIST =	\
	as.h	\
	app.h	\
	arch64_32.h	\
	arm_reloc.h	\
	as.h	\
	atof-ieee.h	\
	bignum.h	\
	dwarf2dbg.h	\
	expr.h	\
	filenames.h	\
	fixes.h	\
	flonum.h	\
	frags.h	\
	hash.h	\
	hex_value.h	\
	hppa-aux.h	\
	hppa-opcode.h	\
	i386.h	\
	i386-opcode.h	\
	i860-opcode.h	\
	input-file.h	\
	input-scrub.h	\
	layout.h	\
	m68k-opcode.h	\
	m88k-opcode.h	\
	md.h	\
	messages.h	\
	obstack.h	\
	ppc-opcode.h	\
	read.h	\
	relax.h	\
	sections.h	\
	sparc-opcode.h	\
	struc-symbol.h	\
	symbols.h	\
	write_object.h	\
	xmalloc.h
