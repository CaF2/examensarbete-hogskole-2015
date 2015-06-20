#makefile
#requires android-sdk apache-ant

PROJ_NAME := WestermoDeviceDisplay2

INSTALL_DIR := ./bin

ANDROID_DIR:=/opt/android-sdk/platform-tools
ADB:=$(ANDROID_DIR)/adb

APP_NAME := $(PROJ_NAME)-debug.apk

all: build.xml
#	ndk-build NDK_DEBUG=1
	ant debug
	
clean:
	ant clean
	ndk-build V=1 APP_ABI=all clean
   
build.xml:
	android update project --target android-19 --name $(PROJ_NAME) --path . --subprojects

install:
	$(ADB) install $(INSTALL_DIR)/$(APP_NAME)
	
flash:
	$(ADB) install -d $(INSTALL_DIR)/$(APP_NAME)
	
reflash:
	$(ADB) install -r -d $(INSTALL_DIR)/$(APP_NAME)

clean:
	ant clean
	rm build.xml
