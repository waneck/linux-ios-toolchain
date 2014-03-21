PROJECTNAME:=TEMPLATE_PROJECT

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

//LDFLAGS += -framework UIKit 
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

FRAMEWORK_HEADER=TEMPLATE_PROJECT.h

all: $(PROJECTNAME) framework

$(PROJECTNAME):	$(OBJS)
	$(AR) cr $@ $(filter %.o,$^)

%.o:	%.m
	$(CC) -c $(CFLAGS) $< -o $@

%.o:	%.c
	$(CC) -c $(CFLAGS) $< -o $@

%.o:	%.cpp
	$(CPP) -c $(CPPFLAGS) $< -o $@

framework: $(OBJS)
	mkdir -p $(PROJECTNAME).framework/Headers
	cp -r $(FRAMEWORK_HEADER) $(PROJECTNAME).framework/Headers
	cp -r $(PROJECTNAME) $(PROJECTNAME).framework

clean:
	find . -name \*.o|xargs rm -rf
	rm -f $(PROJECTNAME)
	rm -f $(PROJECTNAME).framework

.PHONY: all dist install uninstall clean
