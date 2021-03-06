APK_NAME = APKName
PACKAGE_NAME = org.sfmldev.android
KEYS_DIR = ../keys
JNI_NAME = sfml-example

ANDROID_API = 23

JNI_SRCS_C =
JNI_SRCS_CXX = $(shell find jni -name "*.cpp")

JNI_OBJS_ARM = $(patsubst jni/%,build/obj/armeabi/%,$(JNI_SRCS_C:.c=.o) $(JNI_SRCS_CXX:.cpp=.o))
JNI_OBJS_AARCH64 = $(patsubst jni/%,build/obj/arm64-v8a/%,$(JNI_SRCS_C:.c=.o) $(JNI_SRCS_CXX:.cpp=.o))
JNI_OBJS_X86 = $(patsubst jni/%,build/obj/x86/%,$(JNI_SRCS_C:.c=.o) $(JNI_SRCS_CXX:.cpp=.o))
JNI_OBJS_X86_64= $(patsubst jni/%,build/obj/x86_x64/%,$(JNI_SRCS_C:.c=.o) $(JNI_SRCS_CXX:.cpp=.o))
JNI_OBJS_MIPS = $(patsubst jni/%,build/obj/mips/%,$(JNI_SRCS_C:.c=.o) $(JNI_SRCS_CXX:.cpp=.o))
JNI_OBJS_MIPS64 = $(patsubst jni/%,build/obj/mips64/%,$(JNI_SRCS_C:.c=.o) $(JNI_SRCS_CXX:.cpp=.o))

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
APKSIGNER = $(shell which -a apksigner | head -n 1)
ADB = $(shell which -a adb | head -n 1)

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

NDK_TOOLCHAINS = $(NDK)/toolchains/llvm/prebuilt/linux-x86_64
NDK_SYSROOT = $(NDK)/toolchains/llvm/prebuilt/linux-x86_64/sysroot
NDK_BIN = $(NDK)/toolchains/llvm/prebuilt/linux-x86_64/bin/

NDK_LIBS_ARM = $(NDK_SYSROOT)/usr/lib/arm-linux-androideabi
NDK_LIBS_AARCH64 = $(NDK_SYSROOT)/usr/lib/aarch64-linux-android
NDK_LIBS_X86 = $(NDK_SYSROOT)/usr/lib/i686-linux-android
NDK_LIBS_X86_64 = $(NDK_SYSROOT)/usr/lib/x86_64-linux-android
NDK_LIBS_MIPS = $(NDK_SYSROOT)/usr/lib/mipsel-linux-android
NDK_LIBS_MIPS64 = $(NDK_SYSROOT)/usr/lib/mips64el-linux-android

NDK_BIN_armv7a = $(NDK_TOOLCHAINS)/arm-linux-androideabi/bin
NDK_BIN_aarch64 = $(NDK_TOOLCHAINS)/aarch64-linux-android/bin
NDK_BIN_i686 = $(NDK_TOOLCHAINS)/i686-linux-android/bin
NDK_BIN_x86_64 = $(NDK_TOOLCHAINS)/x86_64-linux-android/bin
NDK_BIN_mipsel = $(NDK_TOOLCHAINS)/mipsel-linux-android/bin
NDK_BIN_mips64el = $(NDK_TOOLCHAINS)/mips64el-linux-android/bin

NDK_INCLUDE = $(NDK_SYSROOT)/usr/lib/include

ORG_DIRS = $(subst .,/,$(PACKAGE_NAME))

JNI_CFLAGS = -ggdb -O0 -std=c11 -fPIC -Wall -DDEBUG -I "$(NDK_INCLUDE)"
JNI_CXXFLAGS = -ggdb -O0 -std=c++11 -fPIC -Wall -DANDROID_STL=c++_shared -I "$(NDK_INCLUDE)"
JNI_DEFS = -DANDROID -DDEBUG -DUSE_JNI
JNI_LDFLAGS = -Wl,--as-needed -Wl,--no-undefined -Wl,--no-allow-shlib-undefined
JNI_LIBS = \
	-lsfml-system -lsfml-activity -lsfml-window -lsfml-graphics -lsfml-audio -lsfml-network \
	-lEGL -lGLESv2 -lGLESv3 -lGLESv1_CM \
	-llog -landroid -lm -lz

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
	@test -n "$(NDK_SYSROOT)" -a -d "$(NDK_SYSROOT)" || ( >&2 echo "Package 'google-android-ndk-installer' is needed" ; false )

clean:
	rm -rf build
	rm -f logcat.txt *~

build/gen/$(ORG_DIRS)/R.java: check
	@mkdir -p "build/gen"
	$(AAPT) package -f -m -J "build/gen/" -S res -M "AndroidManifest.xml" -I "$(PLATFORM)"

build/classes/$(ORG_DIRS)/MainActivity.class build/classes/$(ORG_DIRS)/R.class: build/gen/$(ORG_DIRS)/R.java
	@mkdir -p "build/classes"
	$(JAVAC) -bootclasspath "$(SDK)/jre/lib/rt.jar" -classpath "$(PLATFORM)" -d "build/classes" $+

build/apk/classes.dex: build/classes/$(ORG_DIRS)/R.class
	@mkdir -p "build/apk"
	$(DX) --dex --output=build/apk/classes.dex build/classes/


build/obj/armeabi/%.o: jni/%.c
	mkdir -p $(dir $@) && $(CLANG_ARM) $(JNI_DEFS) $(JNI_CFLAGS) -I ../libs/armv7a/include/ -c -o $@ $<

build/obj/armeabi/%.o: jni/%.cpp
	mkdir -p $(dir $@) && $(CLANG_ARM) $(JNI_DEFS) $(JNI_CXXFLAGS) -I ../libs/armv7a/include/ -c -o $@ $<

build/apk/lib/armeabi/lib$(JNI_NAME).so: $(JNI_OBJS_ARM)
	mkdir -p -p $(dir $@) && $(CLANGXX_ARM) -shared -o $@ \
		-Wl,--whole-archive ../libs/armv7a/lib/libsfml-main.a -Wl,--no-whole-archive \
		$+ $(JNI_LDFLAGS) -L ../libs/armv7a/lib/ $(JNI_LIBS)


build/obj/arm64-v8a/%.o: jni/%.c
	mkdir -p $(dir $@) && $(CLANG_AARCH64) $(JNI_DEFS) $(JNI_CFLAGS) -I ../libs/aarch64/include/ -c -o $@ $<

build/obj/arm64-v8a/%.o: jni/%.cpp
	mkdir -p $(dir $@) && $(CLANG_AARCH64) $(JNI_DEFS) $(JNI_CXXFLAGS) -I ../libs/aarch64/include/ -c -o $@ $<

build/apk/lib/arm64-v8a/lib$(JNI_NAME).so: $(JNI_OBJS_AARCH64)
	mkdir -p -p $(dir $@) && $(CLANGXX_AARCH64) -shared -o $@ \
		-Wl,--whole-archive ../libs/aarch64/lib/libsfml-main.a -Wl,--no-whole-archive \
		$+ $(JNI_LDFLAGS) -L ../libs/aarch64/lib/ $(JNI_LIBS)


build/obj/x86/%.o: jni/%.c
	mkdir -p $(dir $@) && $(CLANG_X86) $(JNI_DEFS) $(JNI_CFLAGS) -I ../libs/i686/include/ -c -o $@ $<

build/obj/x86/%.o: jni/%.cpp
	mkdir -p $(dir $@) && $(CLANG_X86) $(JNI_DEFS) $(JNI_CXXFLAGS) -I ../libs/i686/include/ -c -o $@ $<

build/apk/lib/x86/lib$(JNI_NAME).so: $(JNI_OBJS_X86)
	mkdir -p -p $(dir $@) && $(CLANGXX_X86) -shared -o $@ \
		-Wl,--whole-archive ../libs/i686/lib/libsfml-main.a -Wl,--no-whole-archive \
		$+ $(JNI_LDFLAGS) -L ../libs/i686/lib/ $(JNI_LIBS)


build/obj/x86_x64/%.o: jni/%.c
	mkdir -p $(dir $@) && $(CLANG_X86_64) $(JNI_DEFS) $(JNI_CFLAGS) -I ../libs/x86_64/include/ -c -o $@ $<

build/obj/x86_x64/%.o: jni/%.cpp
	mkdir -p $(dir $@) && $(CLANG_X86_64) $(JNI_DEFS) $(JNI_CXXFLAGS) -I ../libs/x86_64/include/ -c -o $@ $<

build/apk/lib/x86_x64/lib$(JNI_NAME).so: $(JNI_OBJS_X86_64)
	mkdir -p -p $(dir $@) && $(CLANGXX_X86_64) -shared -o $@ \
		-Wl,--whole-archive ../libs/x86_64/lib/libsfml-main.a -Wl,--no-whole-archive \
		$+ $(JNI_LDFLAGS) -L ../libs/x86_64/lib/ $(JNI_LIBS)


build/obj/mips/%.o: jni/%.c
	mkdir -p $(dir $@) && $(CLANG_MIPS) $(JNI_DEFS) $(JNI_CFLAGS) -I ../libs/mipsel/include/ -c -o $@ $<

build/obj/mips/%.o: jni/%.cpp
	mkdir -p $(dir $@) && $(CLANG_MIPS) $(JNI_DEFS) $(JNI_CXXFLAGS) -I ../libs/mipsel/include/ -c -o $@ $<

build/apk/lib/mips/lib$(JNI_NAME).so: $(JNI_OBJS_MIPS)
	mkdir -p -p $(dir $@) && $(CLANGXX_MIPS) -shared -o $@ \
		-Wl,--whole-archive ../libs/mipsel/lib/libsfml-main.a -Wl,--no-whole-archive \
		$+ $(JNI_LDFLAGS) -L ../libs/mipsel/lib/ $(JNI_LIBS)


build/obj/mips64/%.o: jni/%.c
	mkdir -p $(dir $@) && $(CLANG_MIPS64) $(JNI_DEFS) $(JNI_CFLAGS) -I ../libs/mips64el/include/ -c -o $@ $<

build/obj/mips64/%.o: jni/%.cpp
	mkdir -p $(dir $@) && $(CLANG_MIPS64) $(JNI_DEFS) $(JNI_CXXFLAGS) -I ../libs/mips64el/include/ -c -o $@ $<

build/apk/lib/mips64/lib$(JNI_NAME).so: $(JNI_OBJS_MIPS64)
	mkdir -p -p $(dir $@) && $(CLANGXX_MIPS64) -shared -o $@ \
		-Wl,--whole-archive ../libs/mips64el/lib/libsfml-main.a -Wl,--no-whole-archive \
		$+ $(JNI_LDFLAGS) -L ../libs/mips64el/lib/ $(JNI_LIBS)

deps:
	mkdir -p build/apk/lib/armeabi/
	install -m 755 $(NDK_LIBS_ARM)/libc++_shared.so build/apk/lib/armeabi/
	install -m 755 ../libs/armv7a/lib/libsfml-*.so build/apk/lib/armeabi/
	install -m 755 ../libs/armv7a/lib/libopenal.so build/apk/lib/armeabi/
	find "build/apk/lib/armeabi/" -type f -name "*.so*" | while read -r FILE; do "$(NDK_BIN_armv7a)/strip" "$$FILE"; done
	mkdir -p build/apk/lib/arm64-v8a/
	install -m 755 $(NDK_LIBS_AARCH64)/libc++_shared.so build/apk/lib/arm64-v8a/
	install -m 755 ../libs/aarch64/lib/libsfml-*.so build/apk/lib/arm64-v8a/
	install -m 755 ../libs/aarch64/lib/libopenal.so build/apk/lib/arm64-v8a/
	find "build/apk/lib/arm64-v8a/" -type f -name "*.so*" | while read -r FILE; do "$(NDK_BIN_aarch64)/strip" "$$FILE"; done
	mkdir -p build/apk/lib/x86/
	install -m 755 $(NDK_LIBS_X86)/libc++_shared.so build/apk/lib/x86/
	install -m 755 ../libs/i686/lib/libsfml-*.so build/apk/lib/x86/
	install -m 755 ../libs/i686/lib/libopenal.so build/apk/lib/x86/
	find "build/apk/lib/x86/" -type f -name "*.so*" | while read -r FILE; do "$(NDK_BIN_i686)/strip" "$$FILE"; done
	mkdir -p build/apk/lib/x86_x64/
	install -m 755 $(NDK_LIBS_X86_64)/libc++_shared.so build/apk/lib/x86_x64/
	install -m 755 ../libs/x86_64/lib/libsfml-*.so build/apk/lib/x86_x64/
	install -m 755 ../libs/x86_64/lib/libopenal.so build/apk/lib/x86_x64/
	find "build/apk/lib/x86_x64/" -type f -name "*.so*" | while read -r FILE; do "$(NDK_BIN_x86_64)/strip" "$$FILE"; done

libs: \
	build/apk/lib/armeabi/lib$(JNI_NAME).so \
	build/apk/lib/arm64-v8a/lib$(JNI_NAME).so \
	build/apk/lib/x86/lib$(JNI_NAME).so \
	build/apk/lib/x86_x64/lib$(JNI_NAME).so


build/$(APK_NAME).unsigned.apk: build/apk/classes.dex deps libs
	@mkdir -p "build/apk"
	$(AAPT) package -f -M "AndroidManifest.xml" -S res/ -I "$(PLATFORM)" -F "build/$(APK_NAME).unsigned.apk" "build/apk/"
	find assets -type f | while read -r FILE; do $(AAPT) add -v "build/$(APK_NAME).unsigned.apk" "$$FILE"; done

build/$(APK_NAME).aligned.apk: build/$(APK_NAME).unsigned.apk
	$(ZIPALIGN) -f -p 4 $< $@

aligned: build/$(APK_NAME).aligned.apk


$(KEYS_DIR)/debug.keystore: check
	if [ ! -e "$@" ]; then $(KEYTOOL) -genkey -v -keystore "$@" -storepass android \
		-alias androiddebugkey -keypass android -keyalg RSA -keysize 2048 -validity 10000 -deststoretype pkcs12 ; fi
	$(KEYTOOL) -list -v -keystore "$@" -storepass android -keypass android


build/$(APK_NAME).apk: build/$(APK_NAME).aligned.apk $(KEYS_DIR)/debug.keystore check
	$(APKSIGNER) sign --ks $(KEYS_DIR)/debug.keystore --ks-key-alias androiddebugkey --out $@ \
		--ks-pass pass:android --key-pass pass:android $<
	$(AAPT) list -v "$<"
	#~ jar tf "$<"

build: build/$(APK_NAME).apk

dump: build
	readelf -Ws build/apk/lib/arm64-v8a/lib$(JNI_NAME).so
	#~ objdump -tT build/apk/lib/arm64-v8a/lib$(JNI_NAME).so

test: build/$(APK_NAME).apk
	$(ADB) logcat -b all -c
	$(ADB) install -r $<
	$(ADB) shell setprop log.redirect-stdio true
	$(ADB) shell am start --activity-clear-top -n "$(PACKAGE_NAME)/android.app.NativeActivity"
	$(ADB) logcat | tee logcat.txt

remove:
	$(ADB) uninstall $(PACKAGE_NAME)

# See: https://source.android.com/devices/tech/debug
# See: https://proandroiddev.com/debugging-native-crashes-in-android-apps-2b86fd7113d8
debug:
	#~ grep -E "#0.* pc" logcat.txt  2>&1 | head -n 5 | sed -e "s|.*pc[ \t]*||g" | while read -r ADDR_FILE; do \
	#~ 	echo "\n$${ADDR_FILE}:" ; \
	#~ 	"$(NDK_BIN)/arm-linux-androideabi-addr2line" -C -f -e "build/apk/lib/armeabi/lib$(JNI_NAME).so" "$$(echo $${ADDR_FILE} | awk '{print $$1}')" ; \
	#~ 	done

	"$(NDK)/prebuilt/linux-x86_64/bin/ndk-stack" --sym "./build/apk/lib/armeabi/" --dump logcat.txt
