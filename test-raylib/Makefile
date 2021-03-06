APK_FILENAME = APKName
PACKAGE_NAME = com.raylib.models_cubicmap
KEYSTORE_DIR = ../keys
JNI_LIB_NAME = app

JDK = /usr/lib/jvm/java-8-openjdk-amd64
SDK = /usr/lib/android-sdk
NDK = /usr/lib/android-ndk

ANDROID_API = 23

SRCS_C = $(shell find src -name "*.c")
SRCS_CXX = $(shell find src -name "*.cpp")
OBJS = $(SRCS_C:.c=.o) $(SRCS_CXX:.cpp=.o)

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

NDK_TOOLCHAINS = $(NDK)/toolchains/llvm/prebuilt/linux-x86_64
NDK_SYSROOT = $(NDK)/toolchains/llvm/prebuilt/linux-x86_64/sysroot
NDK_BIN = $(NDK)/toolchains/llvm/prebuilt/linux-x86_64/bin/

NDK_INCLUDE = $(NDK_SYSROOT)/usr/lib/include

ORG_DIRS = $(subst .,/,$(PACKAGE_NAME))

COMMON_CFLAGS = -ggdb -O0 -g3 -fPIC -Wall -DDEBUG -I "$(NDK_INCLUDE)" \
	-Werror=int-conversion -Werror=implicit-function-declaration
CFLAGS = -std=c17 $(COMMON_CFLAGS)
CXXFLAGS = -std=c++17 $(COMMON_CFLAGS) -DANDROID_STL=c++_shared
DEFS = -DNDEBUG -DASSETS_PREFIX=\"\"
INCS = -I "$(NDK)/sources/android/native_app_glue/"
LDFLAGS = -Wl,--as-needed -Wl,--no-undefined -Wl,--no-allow-shlib-undefined -u ANativeActivity_onCreate
LIBS = -lraylib -lOpenSLES -lEGL -lGLESv3 -llog -landroid -lm -lz

OBJS_armv7a = $(patsubst src/%,build/obj/armeabi/%,$(OBJS))
OBJS_aarch64 = $(patsubst src/%,build/obj/arm64-v8a/%,$(OBJS))
OBJS_i686 = $(patsubst src/%,build/obj/x86/%,$(OBJS))
OBJS_x86_64= $(patsubst src/%,build/obj/x86_x64/%,$(OBJS))
OBJS_mipsel = $(patsubst src/%,build/obj/mips/%,$(OBJS))
OBJS_mips64el = $(patsubst src/%,build/obj/mips64/%,$(OBJS))

CLANG_armv7a = $(NDK)/toolchains/llvm/prebuilt/linux-x86_64/bin/armv7a-linux-androideabi$(ANDROID_API)-clang
CLANG_aarch64 = $(NDK)/toolchains/llvm/prebuilt/linux-x86_64/bin/aarch64-linux-android$(ANDROID_API)-clang
CLANG_i686 = $(NDK)/toolchains/llvm/prebuilt/linux-x86_64/bin/i686-linux-android$(ANDROID_API)-clang
CLANG_x86_64 = $(NDK)/toolchains/llvm/prebuilt/linux-x86_64/bin/x86_64-linux-android$(ANDROID_API)-clang
CLANG_mipsel = $(NDK)/toolchains/llvm/prebuilt/linux-x86_64/bin/mipsel-linux-android$(ANDROID_API)-clang
CLANG_mips64el = $(NDK)/toolchains/llvm/prebuilt/linux-x86_64/bin/mips64el-linux-android$(ANDROID_API)-clang

CLANGXX_armv7a = $(NDK)/toolchains/llvm/prebuilt/linux-x86_64/bin/armv7a-linux-androideabi$(ANDROID_API)-clang++
CLANGXX_aarch64 = $(NDK)/toolchains/llvm/prebuilt/linux-x86_64/bin/aarch64-linux-android$(ANDROID_API)-clang++
CLANGXX_i686 = $(NDK)/toolchains/llvm/prebuilt/linux-x86_64/bin/i686-linux-android$(ANDROID_API)-clang++
CLANGXX_x86_64 = $(NDK)/toolchains/llvm/prebuilt/linux-x86_64/bin/x86_64-linux-android$(ANDROID_API)-clang++
CLANGXX_mipsel = $(NDK)/toolchains/llvm/prebuilt/linux-x86_64/bin/mipsel-linux-android$(ANDROID_API)-clang++
CLANGXX_mips64el = $(NDK)/toolchains/llvm/prebuilt/linux-x86_64/bin/mips64el-linux-android$(ANDROID_API)-clang++

NDK_LIBS_armv7a = $(NDK_SYSROOT)/usr/lib/arm-linux-androideabi
NDK_LIBS_aarch64 = $(NDK_SYSROOT)/usr/lib/aarch64-linux-android
NDK_LIBS_i686 = $(NDK_SYSROOT)/usr/lib/i686-linux-android
NDK_LIBS_x86_64 = $(NDK_SYSROOT)/usr/lib/x86_64-linux-android
NDK_LIBS_mipsel = $(NDK_SYSROOT)/usr/lib/mipsel-linux-android
NDK_LIBS_mips64el = $(NDK_SYSROOT)/usr/lib/mips64el-linux-android

NDK_BIN_armv7a = $(NDK_TOOLCHAINS)/arm-linux-androideabi/bin
NDK_BIN_aarch64 = $(NDK_TOOLCHAINS)/aarch64-linux-android/bin
NDK_BIN_i686 = $(NDK_TOOLCHAINS)/i686-linux-android/bin
NDK_BIN_x86_64 = $(NDK_TOOLCHAINS)/x86_64-linux-android/bin
NDK_BIN_mipsel = $(NDK_TOOLCHAINS)/mipsel-linux-android/bin
NDK_BIN_mips64el = $(NDK_TOOLCHAINS)/mips64el-linux-android/bin

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

build/classes/$(ORG_DIRS)/R.class: build/gen/$(ORG_DIRS)/R.java
	@mkdir -p "build/classes"
	$(JAVAC) -bootclasspath "$(SDK)/jre/lib/rt.jar" -classpath "$(PLATFORM)" -d "build/classes" $+

build/apk/classes.dex: build/classes/$(ORG_DIRS)/R.class
	@mkdir -p "build/apk"
	$(DX) --dex --output=build/apk/classes.dex build/classes/


build/obj/armeabi/%.o: src/%.c
	mkdir -p $(dir $@) && $(CLANG_armv7a) $(DEFS) $(INCS) $(CFLAGS) -I ../libs/armv7a/include/ -c -o $@ $<

build/obj/armeabi/%.o: src/%.cpp
	mkdir -p $(dir $@) && $(CLANG_armv7a) $(DEFS) $(INCS) $(CXXFLAGS) -I ../libs/armv7a/include/ -c -o $@ $<

build/apk/lib/armeabi/lib$(JNI_LIB_NAME).so: $(OBJS_armv7a)
	mkdir -p -p $(dir $@) && $(CLANGXX_armv7a) -shared -o $@ \
		$+ $(LDFLAGS) -L ../libs/armv7a/lib/ $(LIBS)


build/obj/arm64-v8a/%.o: src/%.c
	mkdir -p $(dir $@) && $(CLANG_aarch64) $(DEFS) $(INCS) $(CFLAGS) -I ../libs/aarch64/include/ -c -o $@ $<

build/obj/arm64-v8a/%.o: src/%.cpp
	mkdir -p $(dir $@) && $(CLANG_aarch64) $(DEFS) $(INCS) $(CXXFLAGS) -I ../libs/aarch64/include/ -c -o $@ $<

build/apk/lib/arm64-v8a/lib$(JNI_LIB_NAME).so: $(OBJS_aarch64)
	mkdir -p -p $(dir $@) && $(CLANGXX_aarch64) -shared -o $@ \
		$+ $(LDFLAGS) -L ../libs/aarch64/lib/ $(LIBS)


build/obj/x86/%.o: src/%.c
	mkdir -p $(dir $@) && $(CLANG_i686) $(DEFS) $(INCS) $(CFLAGS) -I ../libs/i686/include/ -c -o $@ $<

build/obj/x86/%.o: src/%.cpp
	mkdir -p $(dir $@) && $(CLANG_i686) $(DEFS) $(INCS) $(CXXFLAGS) -I ../libs/i686/include/ -c -o $@ $<

build/apk/lib/x86/lib$(JNI_LIB_NAME).so: $(OBJS_i686)
	mkdir -p -p $(dir $@) && $(CLANGXX_i686) -shared -o $@ \
		$+ $(LDFLAGS) -L ../libs/i686/lib/ $(LIBS)


build/obj/x86_x64/%.o: src/%.c
	mkdir -p $(dir $@) && $(CLANG_x86_64) $(DEFS) $(INCS) $(CFLAGS) -I ../libs/x86_64/include/ -c -o $@ $<

build/obj/x86_x64/%.o: src/%.cpp
	mkdir -p $(dir $@) && $(CLANG_x86_64) $(DEFS) $(INCS) $(CXXFLAGS) -I ../libs/x86_64/include/ -c -o $@ $<

build/apk/lib/x86_x64/lib$(JNI_LIB_NAME).so: $(OBJS_x86_64)
	mkdir -p -p $(dir $@) && $(CLANGXX_x86_64) -shared -o $@ \
		$+ $(LDFLAGS) -L ../libs/x86_64/lib/ $(LIBS)


build/obj/mips/%.o: src/%.c
	mkdir -p $(dir $@) && $(CLANG_mipsel) $(DEFS) $(INCS) $(CFLAGS) -I ../libs/mipsel/include/ -c -o $@ $<

build/obj/mips/%.o: src/%.cpp
	mkdir -p $(dir $@) && $(CLANG_mipsel) $(DEFS) $(INCS) $(CXXFLAGS) -I ../libs/mipsel/include/ -c -o $@ $<

build/apk/lib/mips/lib$(JNI_LIB_NAME).so: $(OBJS_mipsel)
	mkdir -p -p $(dir $@) && $(CLANGXX_mipsel) -shared -o $@ \
		$+ $(LDFLAGS) -L ../libs/mipsel/lib/ $(LIBS)


build/obj/mips64/%.o: src/%.c
	mkdir -p $(dir $@) && $(CLANG_mips64el) $(DEFS) $(INCS) $(CFLAGS) -I ../libs/mips64el/include/ -c -o $@ $<

build/obj/mips64/%.o: src/%.cpp
	mkdir -p $(dir $@) && $(CLANG_mips64el) $(DEFS) $(INCS) $(CXXFLAGS) -I ../libs/mips64el/include/ -c -o $@ $<

build/apk/lib/mips64/lib$(JNI_LIB_NAME).so: $(OBJS_mips64el)
	mkdir -p -p $(dir $@) && $(CLANGXX_mips64el) -shared -o $@ \
		$+ $(LDFLAGS) -L ../libs/mips64el/lib/ $(LIBS)

deps:
	mkdir -p build/apk/lib/armeabi/
	install -m 755 $(NDK_LIBS_armv7a)/libc++_shared.so build/apk/lib/armeabi/
	find "build/apk/lib/armeabi/" -type f -name "*.so*" | while read -r FILE; do "$(NDK_BIN_armv7a)/strip" "$$FILE"; done
	mkdir -p build/apk/lib/arm64-v8a/
	install -m 755 $(NDK_LIBS_aarch64)/libc++_shared.so build/apk/lib/arm64-v8a/
	find "build/apk/lib/arm64-v8a/" -type f -name "*.so*" | while read -r FILE; do "$(NDK_BIN_aarch64)/strip" "$$FILE"; done
	mkdir -p build/apk/lib/x86/
	install -m 755 $(NDK_LIBS_i686)/libc++_shared.so build/apk/lib/x86/
	find "build/apk/lib/x86/" -type f -name "*.so*" | while read -r FILE; do "$(NDK_BIN_i686)/strip" "$$FILE"; done
	mkdir -p build/apk/lib/x86_x64/
	install -m 755 $(NDK_LIBS_x86_64)/libc++_shared.so build/apk/lib/x86_x64/
	find "build/apk/lib/x86_x64/" -type f -name "*.so*" | while read -r FILE; do "$(NDK_BIN_x86_64)/strip" "$$FILE"; done

libs: \
	build/apk/lib/armeabi/lib$(JNI_LIB_NAME).so \
	build/apk/lib/arm64-v8a/lib$(JNI_LIB_NAME).so \
	build/apk/lib/x86/lib$(JNI_LIB_NAME).so \
	build/apk/lib/x86_x64/lib$(JNI_LIB_NAME).so


build/$(APK_FILENAME).unsigned.apk: build/apk/classes.dex deps libs
	@mkdir -p "build/apk"
	$(AAPT) package -f -M "AndroidManifest.xml" -S res/ -I "$(PLATFORM)" -F "build/$(APK_FILENAME).unsigned.apk" "build/apk/"
	find assets -type f | while read -r FILE; do $(AAPT) add -v "build/$(APK_FILENAME).unsigned.apk" "$$FILE"; done

build/$(APK_FILENAME).aligned.apk: build/$(APK_FILENAME).unsigned.apk
	$(ZIPALIGN) -f -p 4 $< $@

aligned: build/$(APK_FILENAME).aligned.apk


$(KEYSTORE_DIR)/debug.keystore: check
	if [ ! -e "$@" ]; then $(KEYTOOL) -genkey -v -keystore "$@" -storepass android \
		-alias androiddebugkey -keypass android -keyalg RSA -keysize 2048 -validity 10000 -deststoretype pkcs12 ; fi
	$(KEYTOOL) -list -v -keystore "$@" -storepass android -keypass android


build/$(APK_FILENAME).apk: build/$(APK_FILENAME).aligned.apk $(KEYSTORE_DIR)/debug.keystore check
	$(APKSIGNER) sign --ks $(KEYSTORE_DIR)/debug.keystore --ks-key-alias androiddebugkey --out $@ \
		--ks-pass pass:android --key-pass pass:android $<
	$(AAPT) list -v "$<"
	#~ jar tf "$<"

build: build/$(APK_FILENAME).apk

dump: build
	readelf -Ws build/apk/lib/arm64-v8a/lib$(JNI_LIB_NAME).so
	#~ objdump -tT build/apk/lib/arm64-v8a/lib$(JNI_LIB_NAME).so

test: build/$(APK_FILENAME).apk
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
	#~ 	"$(NDK_BIN)/arm-linux-androideabi-addr2line" -C -f -e "build/apk/lib/armeabi/lib$(JNI_LIB_NAME).so" "$$(echo $${ADDR_FILE} | awk '{print $$1}')" ; \
	#~ 	done

	"$(NDK)/prebuilt/linux-x86_64/bin/ndk-stack" --sym "./build/apk/lib/armeabi/" --dump logcat.txt
