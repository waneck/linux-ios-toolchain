IPHONE_IP:=
PROJECTNAME:=TEMPLATE_PROJECT
WEE_BUNDLE_FOLDER:=$(PROJECTNAME).bundle
INSTALLFOLDER:=$(PROJECTNAME).bundle

CC:=ios-clang
CPP:=ios-clang++

CFLAGS += -fobjc-arc
CFLAGS += -fblocks
CFLAGS += -g0 -O2

CPPFLAGS += -objc-arc
CPPFLAGS += -fblocks
CPPFLAGS += -g0 -O2

LDFLAGS += -dynamiclib -lbundle1.o

LDFLAGS += -framework CoreFoundation 
LDFLAGS += -framework Foundation 
LDFLAGS += -framework UIKit 
LDFLAGS += -framework CoreGraphics

SRCDIR1=.
OBJS+=$(patsubst %.m,%.o,$(wildcard $(SRCDIR1)/*.m))
OBJS+=$(patsubst %.c,%.o,$(wildcard $(SRCDIR1)/*.c))
OBJS+=$(patsubst %.c,%.o,$(wildcard $(SRCDIR1)/*.cpp))

INFOPLIST:=$(wildcard *Info.plist)
RESOURCES+=$(wildcard ./images/*)
RESOURCES+=$(wildcard ./Resources/*)
RESOURCES+=$(wildcard ./Localizations/*)


CFLAGS += -I"$(SRCDIR1)"
CPPLAGS += -I"$(SRCDIR1)"

all:	$(PROJECTNAME)

$(PROJECTNAME):	$(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) $(filter %.o,$^) -o $@  

%.o:	%.m
	$(CC) -c $(CFLAGS) $< -o $@

%.o:	%.c
	$(CC) -c $(CFLAGS) $< -o $@

%.o:	%.cpp
	$(CPP) -c $(CPPFLAGS) $< -o $@
langs:
	ios-genLocalization

dist:	$(PROJECTNAME)
	rm -rf $(WEE_BUNDLE_FOLDER)
	mkdir -p $(WEE_BUNDLE_FOLDER)
ifneq ($(RESOURCES),)
	cp -r $(RESOURCES) $(WEE_BUNDLE_FOLDER)
endif
	cp $(INFOPLIST) $(WEE_BUNDLE_FOLDER)/Info.plist
	mv $(PROJECTNAME) $(WEE_BUNDLE_FOLDER)

install: dist
ifeq ($(IPHONE_IP),)
	echo "Please set IPHONE_IP"
else
	ssh root@$(IPHONE_IP) 'rm -fr /System/Library/WeeAppPlugins/$(INSTALLFOLDER)'
	scp -r $(WEE_BUNDLE_FOLDER) root@$(IPHONE_IP):/System/Library/WeeAppPlugins/$(INSTALLFOLDER)
	echo "Application $(INSTALLFOLDER) installed"
	ssh mobile@$(IPHONE_IP) 'killall -9 SpringBoard'
endif

uninstall:
ifeq ($(IPHONE_IP),)
	echo "Please set IPHONE_IP"
else
	ssh root@$(IPHONE_IP) 'rm -fr /System/Library/WeeAppPlugins/$(INSTALLFOLDER)'
	echo "Application $(INSTALLFOLDER) uninstalled"
endif
clean:
	find . -name \*.o|xargs rm -rf
	rm -rf $(WEE_BUNDLE_FOLDER)
	rm -f $(PROJECTNAME)

.PHONY: all dist install uninstall clean
