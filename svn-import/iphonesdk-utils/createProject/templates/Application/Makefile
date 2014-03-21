IPHONE_IP:=
PROJECTNAME:=TEMPLATE_PROJECT
APPFOLDER:=$(PROJECTNAME).app
INSTALLFOLDER:=$(PROJECTNAME).app

CC:=ios-clang
CPP:=ios-clang++

CFLAGS += -objc-arc
CFLAGS += -fblocks
CFLAGS += -g0 -O2
CFLAGS += -I"$(SRCDIR)"

CPPFLAGS += -objc-arc
CPPFLAGS += -fblocks
CPPFLAGS += -g0 -O2
CPPLAGS += -I"$(SRCDIR)"

LDFLAGS += -framework Foundation 
LDFLAGS += -framework UIKit 
//LDFLAGS += -framework CoreGraphics
//LDFLAGS += -framework AVFoundation
//LDFLAGS += -framework AddressBook
//LDFLAGS += -framework AddressBookUI
//LDFLAGS += -framework AudioToolbox
//LDFLAGS += -framework AudioUnit
//LDFLAGS += -framework CFNetwork
//LDFLAGS += -framework CoreAudio
//LDFLAGS += -framework CoreData
//LDFLAGS += -framework CoreFoundation
//LDFLAGS += -framework GraphicsServices
//LDFLAGS += -framework CoreLocation
//LDFLAGS += -framework ExternalAccessory
//LDFLAGS += -framework GameKit
//LDFLAGS += -framework IOKit
//LDFLAGS += -framework MapKit
//LDFLAGS += -framework MediaPlayer
//LDFLAGS += -framework MessageUI
//LDFLAGS += -framework MobileCoreServices
//LDFLAGS += -framework OpenAL
//LDFLAGS += -framework OpenGLES
//LDFLAGS += -framework QuartzCore
//LDFLAGS += -framework Security
//LDFLAGS += -framework StoreKit
//LDFLAGS += -framework System
//LDFLAGS += -framework SystemConfiguration
//LDFLAGS += -framework CoreSurface
//LDFLAGS += -framework GraphicsServices
//LDFLAGS += -framework Celestial
//LDFLAGS += -framework WebCore
//LDFLAGS += -framework WebKit
//LDFLAGS += -framework SpringBoardUI
//LDFLAGS += -framework TelephonyUI
//LDFLAGS += -framework JavaScriptCore
//LDFLAGS += -framework PhotoLibrary

SRCDIR=.
OBJS+=$(patsubst %.m,%.o,$(wildcard $(SRCDIR)/*.m))
OBJS+=$(patsubst %.c,%.o,$(wildcard $(SRCDIR)/*.c))
OBJS+=$(patsubst %.cpp,%.o,$(wildcard $(SRCDIR)/*.cpp))

INFOPLIST:=$(wildcard *Info.plist)

RESOURCES+=$(wildcard ./Images/*)
RESOURCES+=$(wildcard ./Resources/*)
RESOURCES+=$(wildcard ./Localizations/*)


all:	$(PROJECTNAME)

$(PROJECTNAME):	$(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) $(filter %.o,$^) -o $@ 

%.o:	%.m
	$(CC) -c $(CFLAGS) $< -o $@

%.o:	%.c
	$(CC) -c $(CFLAGS) $< -o $@

%.o:	%.cpp
	$(CPP) -c $(CPPFLAGS) $< -o $@


dist:	$(PROJECTNAME)
	mkdir -p $(APPFOLDER)
ifneq ($(RESOURCES),)
	cp -r $(RESOURCES) $(APPFOLDER)
endif
	cp $(INFOPLIST) $(APPFOLDER)/Info.plist
	cp $(PROJECTNAME) $(APPFOLDER)
	find $(APPFOLDER) -name \*.png|xargs ios-pngcrush -c
	find $(APPFOLDER) -name \*.plist|xargs ios-plutil -c
	find $(APPFOLDER) -name \*.strings|xargs ios-plutil -c

langs:
	ios-genLocalization

install: dist
ifeq ($(IPHONE_IP),)
	echo "Please set IPHONE_IP"
else
	ssh root@$(IPHONE_IP) 'rm -fr /Applications/$(INSTALLFOLDER)'
	scp -r $(APPFOLDER) root@$(IPHONE_IP):/Applications/$(INSTALLFOLDER)
	echo "Application $(INSTALLFOLDER) installed"
	ssh mobile@$(IPHONE_IP) 'uicache'
endif

uninstall:
ifeq ($(IPHONE_IP),)
	echo "Please set IPHONE_IP"
else
	ssh root@$(IPHONE_IP) 'rm -fr /Applications/$(INSTALLFOLDER)'
	echo "Application $(INSTALLFOLDER) uninstalled"
endif
clean:
	find . -name \*.o|xargs rm -rf
	rm -rf $(APPFOLDER)
	rm -f $(PROJECTNAME)

.PHONY: all dist install uninstall clean
