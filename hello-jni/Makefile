APK_NAME = APKName
PACKAGE_NAME = com.example.hellojni
KEYS_DIR = ../keys

JNI_SRCS = $(shell find jni -name "*.c")
JNI_CFLAGS = -fPIC -pedantic -Wall -Wno-unused-value -Wno-unused-function
JNI_LDFLAGS = -Wl,--as-needed -Wl,--no-undefined -Wl,--no-allow-shlib-undefined
JNI_LIBS = -llog -landroid -lGLESv2
JNI_NAME = hello-jni

JDK = /usr/lib/jvm/java-8-openjdk-amd64
SDK = /usr/lib/android-sdk
NDK = /usr/lib/android-ndk

JAVAC = $(JDK)/bin/javac
JAVAH = $(JDK)/bin/javah
KEYTOOL = $(JDK)/bin/keytool

BUILD_TOOLS_DIR = $(SDK)/build-tools/debian
AAPT = $(BUILD_TOOLS_DIR)/aapt
DX = $(BUILD_TOOLS_DIR)/dx
ZIPALIGN = $(BUILD_TOOLS_DIR)/zipalign
APKSIGNER = $(shell which -a apksigner)
ADB = $(shell which -a adb)

ANDROID_API = 23
PLATFORM = /usr/lib/android-sdk/platforms/android-$(ANDROID_API)/android.jar

CLANG_ARM = $(NDK)/toolchains/llvm/prebuilt/linux-x86_64/bin/armv7a-linux-androideabi$(ANDROID_API)-clang
CLANG_AARCH64 = $(NDK)/toolchains/llvm/prebuilt/linux-x86_64/bin/aarch64-linux-android$(ANDROID_API)-clang
CLANG_X86 = $(NDK)/toolchains/llvm/prebuilt/linux-x86_64/bin/i686-linux-android$(ANDROID_API)-clang
CLANG_X86_64 = $(NDK)/toolchains/llvm/prebuilt/linux-x86_64/bin/x86_64-linux-android$(ANDROID_API)-clang
CLANG_MIPS = $(NDK)/toolchains/llvm/prebuilt/linux-x86_64/bin/mipsel-linux-android$(ANDROID_API)-clang
CLANG_MIPS64 = $(NDK)/toolchains/llvm/prebuilt/linux-x86_64/bin/mips64el-linux-android$(ANDROID_API)-clang

ORG_DIRS = $(subst .,/,$(PACKAGE_NAME))

all: test

check:
	@test -n "$(JDK)" -a -d "$(JDK)" || ( >&2 echo "Package 'openjdk-8-jdk' is needed" ; false )
	@test -n "$(SDK)" -a -d "$(SDK)" || ( >&2 echo "Package 'android-sdk' is needed" ; false )
	@test -n "$(NDK)" -a -d "$(NDK)" || ( >&2 echo "Package 'google-android-ndk-installer' is needed" ; false )
	@test -n "$(JAVAC)" -o ! -x "$(JAVAC)" || ( >&2 echo "Package 'openjdk-8-jdk' is needed" ; false )
	@test -n "$(JAVAH)" -o ! -x "$(JAVAH)" || ( >&2 echo "Package 'openjdk-8-jdk' is needed" ; false )
	@test -n "$(KEYTOOL)" -o ! -x "$(KEYTOOL)" || ( >&2 echo "Package 'openjdk-8-jre' is needed" ; false )
	@test -n "$(BUILD_TOOLS_DIR)" -a -d "$(BUILD_TOOLS_DIR)" || ( >&2 echo "Package 'android-sdk' is needed" ; false )
	@test -n "$(AAPT)" -o ! -x "$(AAPT)" || ( >&2 echo "Package 'aapt' is needed" ; false )
	@test -n "$(DX)" -o ! -x "$(DX)" || ( >&2 echo "Package 'dalvik-exchange' is needed" ; false )
	@test -n "$(ZIPALIGN)" -o ! -x "$(ZIPALIGN)" || ( >&2 echo "Package 'zipalign' is needed" ; false )
	@test -n "$(APKSIGNER)" -o ! -x "$(APKSIGNER)" || ( >&2 echo "Package 'apksigner' is needed" ; false )
	@test -n "$(ADB)" -o ! -x "$(ADB)" || ( >&2 echo "Package 'adb' is needed" ; false )
	@test -n "$(PLATFORM)" -o ! -r "$(PLATFORM)" || ( >&2 echo "Package 'libandroid-$(ANDROID_API)-java' is needed" ; false )

clean:
	rm -rf build

# Generate R.java file with Android Asset Packaging Tool (aapt)
################################################################

#~ The -f flag serves to aapt to overwrite any existing output file.
#~ The -m causes it to create package directories under the output directory.
#~ The -J makes it generate the R.java file and sets the output directory.
#~ The -S points out the resource directory. The -M specifies the manifest.
#~ The -I adds the platform .jar as an "include file".
#~ It all creates: build/gen/"${ORG_DIRS}"/R.java.

build/gen/$(ORG_DIRS)/R.java: check
	@mkdir -p "build/gen"
	$(AAPT) package -f -m -J "build/gen/" -S res -M "AndroidManifest.xml" -I "$(PLATFORM)"

# Compile the java files with javac
####################################

build/obj/$(ORG_DIRS)/MainActivity.class build/obj/$(ORG_DIRS)/R.class: build/gen/$(ORG_DIRS)/R.java
	@mkdir -p "build/obj"
	$(JAVAC) -bootclasspath "$(SDK)/jre/lib/rt.jar" -classpath "$(PLATFORM)" -d "build/obj" $+ "src/$(ORG_DIRS)"/*.java

# Translate the .class files in build/obj/ to Dalvik byte code with dx tool
############################################################################

build/apk/classes.dex: build/obj/$(ORG_DIRS)/R.class
	@mkdir -p "build/apk"
	$(DX) --dex --output=build/apk/classes.dex build/obj/

# Search the C function signature that corresponds to the Java method with javah tool
######################################################################################

build/tmp/jni.h: build/obj/$(ORG_DIRS)/MainActivity.class
	@mkdir -p "build/tmp"
	$(JAVAH) -classpath "$(PLATFORM):build/obj" -o "build/tmp/jni.h" "$(PACKAGE_NAME).MainActivity"
	grep -A1 _getMessage "build/tmp/jni.h"

# Create the libhello.so shared objects with the toolchains provided
#####################################################################

build/apk/lib: $(JNI_SRCS)
	@mkdir -p "build/apk/lib"
	if [ -x $(CLANG_ARM) ]; then \
		mkdir -p build/apk/lib/armeabi && $(CLANG_ARM) -shared $(JNI_CFLAGS) -o build/apk/lib/armeabi/lib$(JNI_NAME).so $(JNI_SRCS) $(JNI_LDFLAGS) $(JNI_LIBS) ; fi
	if [ -x $(CLANG_AARCH64) ]; then \
		mkdir -p build/apk/lib/arm64-v8a && $(CLANG_AARCH64) -shared $(JNI_CFLAGS) -o build/apk/lib/arm64-v8a/lib$(JNI_NAME).so $(JNI_SRCS) $(JNI_LDFLAGS) $(JNI_LIBS) ; fi
	if [ -x $(CLANG_X86) ]; then \
		mkdir -p build/apk/lib/x86 && $(CLANG_X86) -shared $(JNI_CFLAGS) -o build/apk/lib/x86/lib$(JNI_NAME).so $(JNI_SRCS) $(JNI_LDFLAGS) $(JNI_LIBS) ; fi
	if [ -x $(CLANG_X86_64) ]; then \
		mkdir -p build/apk/lib/x86_x64 && $(CLANG_X86_64) -shared $(JNI_CFLAGS) -o build/apk/lib/x86_x64/lib$(JNI_NAME).so $(JNI_SRCS) $(JNI_LDFLAGS) $(JNI_LIBS) ; fi
	if [ -x $(CLANG_MIPS) ]; then \
		mkdir -p build/apk/lib/mips && $(CLANG_MIPS) -shared $(JNI_CFLAGS) -o build/apk/lib/mips/lib$(JNI_NAME).so $(JNI_SRCS) $(JNI_LDFLAGS) $(JNI_LIBS) ; fi
	if [ -x $(CLANG_MIPS64) ]; then \
		mkdir -p build/apk/lib/mips64 && $(CLANG_MIPS64) -shared $(JNI_CFLAGS) -o build/apk/lib/mips64/lib$(JNI_NAME).so $(JNI_SRCS) $(JNI_LDFLAGS) $(JNI_LIBS) ; fi

# Package to create APK using the aapt tool 
############################################

build/$(APK_NAME).unsigned.apk: build/apk/classes.dex build/apk/lib
	@mkdir -p "build/apk"
	$(AAPT) package -f -M "AndroidManifest.xml" -S res/ -I "$(PLATFORM)" -F "build/$(APK_NAME).unsigned.apk" "build/apk/"

# Use zipalign tool in the APK
###############################

build/$(APK_NAME).aligned.apk: build/$(APK_NAME).unsigned.apk
	$(ZIPALIGN) -f -p 4 $< $@

aligned: build/$(APK_NAME).aligned.apk

# Create a key store and key for signing with the Java keytool
###############################################################

$(KEYS_DIR)/debug.keystore: check
	if [ ! -e "$@" ]; then $(KEYTOOL) -genkey -v -keystore "$@" -storepass android \
		-alias androiddebugkey -keypass android -keyalg RSA -keysize 2048 -validity 10000 -deststoretype pkcs12 ; fi
	$(KEYTOOL) -list -v -keystore "$@" -storepass android -keypass android

# Sign the APK with apksigner tool
###################################

build/$(APK_NAME).apk: build/$(APK_NAME).aligned.apk $(KEYS_DIR)/debug.keystore check
	$(APKSIGNER) sign --ks $(KEYS_DIR)/debug.keystore --ks-key-alias androiddebugkey --out $@ \
		--ks-pass pass:android --key-pass pass:android $<

# Check the APK content with aapt tool or jar tool
###################################################

list: build/$(APK_NAME).apk
	# with aapt
	$(AAPT) list $<

	# aapt with more details
	$(AAPT) list -v $<

	# with jar
	jar tf $<


# Test app with the adb tool
#############################

test: build/$(APK_NAME).apk
	$(ADB) install -r $<

	# The --activity-clear-top is for clean the task avoiding the warning "Warning: Activity not started, its current task has been brought to the front".
	$(ADB) shell am start --activity-clear-top -n "$(PACKAGE_NAME)/.HelloJni"
