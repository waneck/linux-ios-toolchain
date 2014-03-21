IPHONE_IP:=
PROJECTNAME:=TEMPLATE_PROJECT

CC:=ios-clang
CPP:=ios-clang++

CFLAGS += -objc-arc
CFLAGS += -fblocks
CFLAGS += -g0 -O2

CPPFLAGS += -objc-arc
CPPFLAGS += -fblocks
CPPFLAGS += -g0 -O2

LDFLAGS += -framework Foundation

SRCDIR=.
OBJS+=$(patsubst %.m,%.o,$(wildcard $(SRCDIR)/*.m))
OBJS+=$(patsubst %.c,%.o,$(wildcard $(SRCDIR)/*.c))
OBJS+=$(patsubst %.cpp,%.o,$(wildcard $(SRCDIR)/*.cpp))

all:	$(PROJECTNAME)

$(PROJECTNAME):	$(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) $(filter %.o,$^) -o $@ 

%.o:	%.m
	$(CC) -c $(CFLAGS) $< -o $@

%.o:	%.c
	$(CC) -c $(CFLAGS) $< -o $@

%.o:	%.cpp
	$(CPP) -c $(CPPFLAGS) $< -o $@

clean:
	find . -name \*.o|xargs rm -rf
	rm -f $(PROJECTNAME)

.PHONY: all clean
