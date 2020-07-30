ANDROID_API = 23
KEYS_DIR = ../keys

ORG_DIRS = $(subst .,/,$(PACKAGE_NAME))

OBJS_ARM = $(addprefix build/obj/armeabi/,$(OBJS))
OBJS_AARCH64 = $(addprefix build/obj/arm64-v8a/,$(OBJS))
OBJS_X86 = $(addprefix build/obj/x86/,$(OBJS))
OBJS_X86_64= $(addprefix build/obj/x86_x64/,$(OBJS))
OBJS_MIPS = $(addprefix build/obj/mips/,$(OBJS))
OBJS_MIPS64 = $(addprefix build/obj/mips64/,$(OBJS))

JNI_CFLAGS = -std=c11 -fPIC -Wall -Wdate-time -D_FORTIFY_SOURCE=1
JNI_CXXFLAGS = -std=c++11 -fno-rtti -fno-exceptions -fPIC -Wall -Wdate-time -D_FORTIFY_SOURCE=1
JNI_DEFS = -DDEBUG -DANDROID_STL=c++_shared
JNI_LDFLAGS = -Wl,--as-needed -Wl,--no-undefined -Wl,--no-allow-shlib-undefined
JNI_LIBS = -llog -landroid -lEGL -lGLESv2 -lGLESv3 -lm
JNI_NAME = gles3jni

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

PLATFORM = /usr/lib/android-sdk/platforms/android-$(ANDROID_API)/android.jar

CLANG_ARM = $(NDK)/toolchains/llvm/prebuilt/linux-x86_64/bin/armv7a-linux-androideabi$(ANDROID_API)-clang
CLANG_AARCH64 = $(NDK)/toolchains/llvm/prebuilt/linux-x86_64/bin/aarch64-linux-android$(ANDROID_API)-clang
CLANG_X86 = $(NDK)/toolchains/llvm/prebuilt/linux-x86_64/bin/i686-linux-android$(ANDROID_API)-clang
CLANG_X86_64 = $(NDK)/toolchains/llvm/prebuilt/linux-x86_64/bin/x86_64-linux-android$(ANDROID_API)-clang
CLANG_MIPS = $(NDK)/toolchains/llvm/prebuilt/linux-x86_64/bin/mipsel-linux-android$(ANDROID_API)-clang
CLANG_MIPS64 = $(NDK)/toolchains/llvm/prebuilt/linux-x86_64/bin/mips64el-linux-android$(ANDROID_API)-clang

CLANGXX_ARM = $(NDK)/toolchains/llvm/prebuilt/linux-x86_64/bin/armv7a-linux-androideabi$(ANDROID_API)-clang++
CLANGXX_AARCH64 = $(NDK)/toolchains/llvm/prebuilt/linux-x86_64/bin/aarch64-linux-android$(ANDROID_API)-clang++
CLANGXX_X86 = $(NDK)/toolchains/llvm/prebuilt/linux-x86_64/bin/i686-linux-android$(ANDROID_API)-clang++
CLANGXX_X86_64 = $(NDK)/toolchains/llvm/prebuilt/linux-x86_64/bin/x86_64-linux-android$(ANDROID_API)-clang++
CLANGXX_MIPS = $(NDK)/toolchains/llvm/prebuilt/linux-x86_64/bin/mipsel-linux-android$(ANDROID_API)-clang++
CLANGXX_MIPS64 = $(NDK)/toolchains/llvm/prebuilt/linux-x86_64/bin/mips64el-linux-android$(ANDROID_API)-clang++

NDK_SYSROOT = $(NDK)/toolchains/llvm/prebuilt/linux-x86_64/sysroot
NDK_LIBS_ARM = $(NDK_SYSROOT)/usr/lib/arm-linux-androideabi
NDK_LIBS_AARCH64 = $(NDK_SYSROOT)/usr/lib/aarch64-linux-android
NDK_LIBS_X86 = $(NDK_SYSROOT)/usr/lib/i686-linux-android
NDK_LIBS_X86_64 = $(NDK_SYSROOT)/usr/lib/x86_64-linux-android
NDK_LIBS_MIPS = $(NDK_SYSROOT)/usr/lib/mipsel-linux-android
NDK_LIBS_MIPS64 = $(NDK_SYSROOT)/usr/lib/mips64el-linux-android

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
	@test -n "$(NDK_SYSROOT)" -a -d "$(NDK_SYSROOT)" || ( >&2 echo "Package 'google-android-ndk-installer' is needed" ; false )

build/obj/armeabi/%.o: src/%.c
	mkdir -p $(dir $@) && $(CLANG_ARM) $(JNI_DEFS) $(JNI_CFLAGS) -c -o $@ $<

build/obj/armeabi/%.o: src/%.cpp
	mkdir -p $(dir $@) && $(CLANG_ARM) $(JNI_DEFS) $(JNI_CXXFLAGS) -c -o $@ $<

build/apk/lib/armeabi/lib$(JNI_NAME).so: $(OBJS_ARM)
	mkdir -p -p $(dir $@) && $(CLANGXX_ARM) -shared -o $@ $+ $(JNI_LDFLAGS) $(JNI_LIBS)


build/obj/arm64-v8a/%.o: src/%.c
	mkdir -p $(dir $@) && $(CLANG_AARCH64) $(JNI_DEFS) $(JNI_CFLAGS) -c -o $@ $<

build/obj/arm64-v8a/%.o: src/%.cpp
	mkdir -p $(dir $@) && $(CLANG_AARCH64) $(JNI_DEFS) $(JNI_CXXFLAGS) -c -o $@ $<

build/apk/lib/arm64-v8a/lib$(JNI_NAME).so: $(OBJS_AARCH64)
	mkdir -p -p $(dir $@) && $(CLANGXX_AARCH64) -shared -o $@ $+ $(JNI_LDFLAGS) $(JNI_LIBS)


build/obj/x86/%.o: src/%.c
	mkdir -p $(dir $@) && $(CLANG_X86) $(JNI_DEFS) $(JNI_CFLAGS) -c -o $@ $<

build/obj/x86/%.o: src/%.cpp
	mkdir -p $(dir $@) && $(CLANG_X86) $(JNI_DEFS) $(JNI_CXXFLAGS) -c -o $@ $<

build/apk/lib/x86/lib$(JNI_NAME).so: $(OBJS_X86)
	mkdir -p -p $(dir $@) && $(CLANGXX_X86) -shared -o $@ $+ $(JNI_LDFLAGS) $(JNI_LIBS)


build/obj/x86_x64/%.o: src/%.c
	mkdir -p $(dir $@) && $(CLANG_X86_64) $(JNI_DEFS) $(JNI_CFLAGS) -c -o $@ $<

build/obj/x86_x64/%.o: src/%.cpp
	mkdir -p $(dir $@) && $(CLANG_X86_64) $(JNI_DEFS) $(JNI_CXXFLAGS) -c -o $@ $<

build/apk/lib/x86_x64/lib$(JNI_NAME).so: $(OBJS_X86_64)
	mkdir -p -p $(dir $@) && $(CLANGXX_X86_64) -shared -o $@ $+ $(JNI_LDFLAGS) $(JNI_LIBS)


build/obj/mips/%.o: src/%.c
	mkdir -p $(dir $@) && $(CLANG_MIPS) $(JNI_DEFS) $(JNI_CFLAGS) -c -o $@ $<

build/obj/mips/%.o: src/%.cpp
	mkdir -p $(dir $@) && $(CLANG_MIPS) $(JNI_DEFS) $(JNI_CXXFLAGS) -c -o $@ $<

build/apk/lib/mips/lib$(JNI_NAME).so: $(OBJS_MIPS)
	mkdir -p -p $(dir $@) && $(CLANGXX_MIPS) -shared -o $@ $+ $(JNI_LDFLAGS) $(JNI_LIBS)


build/obj/mips64/%.o: src/%.c
	mkdir -p $(dir $@) && $(CLANG_MIPS64) $(JNI_DEFS) $(JNI_CFLAGS) -c -o $@ $<

build/obj/mips64/%.o: src/%.cpp
	mkdir -p $(dir $@) && $(CLANG_MIPS64) $(JNI_DEFS) $(JNI_CXXFLAGS) -c -o $@ $<

build/apk/lib/mips64/lib$(JNI_NAME).so: $(OBJS_MIPS64)
	mkdir -p -p $(dir $@) && $(CLANGXX_MIPS64) -shared -o $@ $+ $(JNI_LDFLAGS) $(JNI_LIBS)


$(KEYS_DIR)/debug.keystore:
	if [ ! -e "$@" ]; then $(KEYTOOL) -genkey -v -keystore "$@" -storepass android \
		-alias androiddebugkey -keypass android -keyalg RSA -keysize 2048 -validity 10000 -deststoretype pkcs12 ; fi
	$(KEYTOOL) -list -v -keystore "$@" -storepass android -keypass android


build/gen/$(ORG_DIRS)/R.java: check
	@mkdir -p "build/gen"
	$(AAPT) package -f -m -J "build/gen/" -S "java/res" -M "AndroidManifest.xml" -I "$(PLATFORM)"

build/classes/$(ORG_DIRS)/R.class: build/gen/$(ORG_DIRS)/R.java $(JAVA_SOURCES)
	@mkdir -p "build/classes"
	$(JAVAC) -bootclasspath "$(SDK)/jre/lib/rt.jar" -classpath "$(PLATFORM)" -d "build/classes" $+

build/apk/classes.dex: build/classes/$(ORG_DIRS)/R.class
	@mkdir -p "build/apk"
	$(DX) --dex --output=build/apk/classes.dex build/classes/


deps:
	mkdir -p build/apk/lib/armeabi/
	install -m 755 $(NDK_LIBS_ARM)/libc++_shared.so build/apk/lib/armeabi/
	mkdir -p build/apk/lib/arm64-v8a/
	install -m 755 $(NDK_LIBS_AARCH64)/libc++_shared.so build/apk/lib/arm64-v8a/
	mkdir -p build/apk/lib/x86/
	install -m 755 $(NDK_LIBS_X86)/libc++_shared.so build/apk/lib/x86/
	mkdir -p build/apk/lib/x86_x64/
	install -m 755 $(NDK_LIBS_X86_64)/libc++_shared.so build/apk/lib/x86_x64/

libs: \
	build/apk/lib/armeabi/lib$(JNI_NAME).so \
	build/apk/lib/arm64-v8a/lib$(JNI_NAME).so \
	build/apk/lib/x86/lib$(JNI_NAME).so \
	build/apk/lib/x86_x64/lib$(JNI_NAME).so


build/$(APK_NAME).unaligned.apk: build/apk/classes.dex deps libs
	@mkdir -p "build/apk"
	$(AAPT) package -f -M "AndroidManifest.xml" -S "java/res" -I "$(PLATFORM)" -F "$@" "build/apk/"

build/$(APK_NAME).unsigned.apk: build/$(APK_NAME).unaligned.apk
	$(ZIPALIGN) -f -p 4 "$<" "$@"

build/$(APK_NAME).apk: build/$(APK_NAME).unsigned.apk $(KEYS_DIR)/debug.keystore check
	$(APKSIGNER) sign --ks $(KEYS_DIR)/debug.keystore --ks-key-alias androiddebugkey --out "$@" \
		--ks-pass pass:android --key-pass pass:android "$<"


apk: check build/$(APK_NAME).apk

test: build/$(APK_NAME).apk
	#~ $(ADB) logcat -b all -c
	$(ADB) install -r "$<"
	#~ $(ADB) shell setprop log.redirect-stdio true
	$(ADB) shell am start --activity-clear-top -n "$(PACKAGE_NAME)/.$(ACTIVITY_NAME)"
	#~ $(ADB) logcat | tee logcat.txt

list: check build/$(APK_NAME).apk
	# with aapt
	#~ $(AAPT) list $<

	# aapt with more details
	$(AAPT) list -v $<

	# with jar
	#~ jar tf $<

build/tmp/jni.h: build/classes/$(ORG_DIRS)/$(ACTIVITY_NAME).class
	@mkdir -p "build/tmp"
	$(JAVAH) -classpath "$(PLATFORM):build/obj" -o "build/tmp/jni.h" "$(PACKAGE_NAME).$(ACTIVITY_NAME)"
	grep -A1 _getMessage "build/tmp/jni.h"
