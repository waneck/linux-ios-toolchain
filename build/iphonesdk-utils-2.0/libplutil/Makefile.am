noinst_LTLIBRARIES=libplutil.la

libplutil_la_CFLAGS = $(XML_CFLAGS)

#this must check
libplutil_la_CFLAGS += -DPLIST_BYTE_ORDER=0 
libplutil_la_LIBADD = $(XML_LIBS)

libplutil_la_SOURCES=base64.c \
	bplist.c \
	bytearray.c \
	hashtable.c \
	iterator.c \
	list.c \
	node.c \
	node_iterator.c \
	node_list.c \
	plist.c \
	ptrarray.c \
	xplist.c

noinst_HEADERS =	\
	base64.h	\
	bytearray.h	\
	common.h	\
	hashtable.h	\
	iterator.h	\
	list.h	\
	node.h	\
	node_iterator.h	\
	node_list.h	\
	object.h	\
	plist.h	\
	plutil.h	\
	ptrarray.h \
  plist/Array.h \
  plist/Boolean.h \
  plist/Data.h \
  plist/Date.h \
  plist/Dictionary.h \
  plist/Integer.h \
  plist/Node.h \
  plist/plist.h \
  plist/plist++.h \
  plist/Real.h \
  plist/String.h \
  plist/Structure.h

