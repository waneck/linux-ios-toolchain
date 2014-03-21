PROJECTNAME:=libTEMPLATE_PROJECT

CC:=ios-clang
CPP:=ios-clang++
AR:=arm-apple-darwin11-ar

CFLAGS += -objc-arc
CFLAGS += -fblocks
CFLAGS += -g0 -O2
CFLAGS += -I"$(SRCDIR)"

CPPFLAGS += -objc-arc
CPPFLAGS += -fblocks
CPPFLAGS += -g0 -O2
CPPLAGS += -I"$(SRCDIR)"

SRCDIR=.
OBJS+=$(patsubst %.m,%.o,$(wildcard $(SRCDIR)/*.m))
OBJS+=$(patsubst %.c,%.o,$(wildcard $(SRCDIR)/*.c))
OBJS+=$(patsubst %.cpp,%.o,$(wildcard $(SRCDIR)/*.cpp))

all:	$(PROJECTNAME)

$(PROJECTNAME):	$(OBJS)
	$(AR) cr $@.a $(filter %.o,$^)

%.o:	%.m
	$(CC) -c $(CFLAGS) $< -o $@

%.o:	%.c
	$(CC) -c $(CFLAGS) $< -o $@

%.o:	%.cpp
	$(CPP) -c $(CPPFLAGS) $< -o $@


clean:
	find . -name \*.o|xargs rm -rf
	rm -f $(PROJECTNAME).a

.PHONY: all dist install uninstall clean
