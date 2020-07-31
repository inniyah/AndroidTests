ROOTDIR := $(dir $(abspath $(lastword $(MAKEFILE_LIST))))
CACHEDIR := cache
BUILDDIR := build
INSTALLDIR := build/output
DEPLOYDIR := libs

CFLAGS = -std=c11 -fPIC -Wall -Wdate-time -D_FORTIFY_SOURCE=1
CXXFLAGS = -std=c++11 -fPIC -Wall -Wdate-time -D_FORTIFY_SOURCE=1
LDFLAGS = -Wl,--as-needed -Wl,--no-undefined -Wl,--no-allow-shlib-undefined

ANDROID_API = 23

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

NDK_TOOLCHAINS = $(NDK)/toolchains/llvm/prebuilt/linux-x86_64

CC_armv7a = $(NDK_TOOLCHAINS)/bin/armv7a-linux-androideabi$(ANDROID_API)-clang
CC_aarch64 = $(NDK_TOOLCHAINS)/bin/aarch64-linux-android$(ANDROID_API)-clang
CC_i686 = $(NDK_TOOLCHAINS)/bin/i686-linux-android$(ANDROID_API)-clang
CC_x86_64 = $(NDK_TOOLCHAINS)/bin/x86_64-linux-android$(ANDROID_API)-clang
CC_mipsel = $(NDK_TOOLCHAINS)/bin/mipsel-linux-android$(ANDROID_API)-clang
CC_mips64el = $(NDK_TOOLCHAINS)/bin/mips64el-linux-android$(ANDROID_API)-clang

CXX_armv7a = $(NDK_TOOLCHAINS)/bin/armv7a-linux-androideabi$(ANDROID_API)-clang++
CXX_aarch64 = $(NDK_TOOLCHAINS)/bin/aarch64-linux-android$(ANDROID_API)-clang++
CXX_i686 = $(NDK_TOOLCHAINS)/bin/i686-linux-android$(ANDROID_API)-clang++
CXX_x86_64 = $(NDK_TOOLCHAINS)/bin/x86_64-linux-android$(ANDROID_API)-clang++
CXX_mipsel = $(NDK_TOOLCHAINS)/bin/mipsel-linux-android$(ANDROID_API)-clang++
CXX_mips64el = $(NDK_TOOLCHAINS)/bin/mips64el-linux-android$(ANDROID_API)-clang++

NDK_BIN_armv7a = $(NDK_TOOLCHAINS)/arm-linux-androideabi/bin
NDK_BIN_aarch64 = $(NDK_TOOLCHAINS)/aarch64-linux-android/bin
NDK_BIN_i686 = $(NDK_TOOLCHAINS)/i686-linux-android/bin
NDK_BIN_x86_64 = $(NDK_TOOLCHAINS)/x86_64-linux-android/bin
NDK_BIN_mipsel = $(NDK_TOOLCHAINS)/mipsel-linux-android/bin
NDK_BIN_mips64el = $(NDK_TOOLCHAINS)/mips64el-linux-android/bin

HOST_armv7a = arm-linux
HOST_aarch64 = aarch64-linux
HOST_i686 = i686-linux
HOST_x86_64 = x86_64-linux
HOST_mipsel = mips-linux
HOST_mips64el = mips64-linux

NDK_SYSROOT = $(NDK_TOOLCHAINS)/sysroot
NDK_LIBS_armv7a = $(NDK_SYSROOT)/usr/lib/arm-linux-androideabi
NDK_LIBS_aarch64 = $(NDK_SYSROOT)/usr/lib/aarch64-linux-android
NDK_LIBS_i686 = $(NDK_SYSROOT)/usr/lib/i686-linux-android
NDK_LIBS_x86_64 = $(NDK_SYSROOT)/usr/lib/x86_64-linux-android
NDK_LIBS_mipsel = $(NDK_SYSROOT)/usr/lib/mipsel-linux-android
NDK_LIBS_mips64el = $(NDK_SYSROOT)/usr/lib/mips64el-linux-android

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

$(CACHEDIR)/flac-1.3.3.tar.xz:
	@mkdir -p $(CACHEDIR)
	wget -nc -P $(CACHEDIR) http://downloads.xiph.org/releases/flac/$(notdir $@)

$(CACHEDIR)/libvorbis-1.3.7.tar.xz:
	@mkdir -p $(CACHEDIR)
	wget -nc -P $(CACHEDIR) https://ftp.osuosl.org/pub/xiph/releases/vorbis/$(notdir $@)

$(CACHEDIR)/libogg-1.3.4.tar.xz:
	@mkdir -p $(CACHEDIR)
	wget -nc -P $(CACHEDIR) https://ftp.osuosl.org/pub/xiph/releases/ogg/$(notdir $@)

$(CACHEDIR)/freetype-2.10.2.tar.xz:
	@mkdir -p $(CACHEDIR)
	wget -nc -P $(CACHEDIR) http://download.savannah.gnu.org/releases/freetype/$(notdir $@)

$(CACHEDIR)/openal-soft-1.20.1.tar.bz2:
	wget -nc -P $(CACHEDIR) https://kcat.strangesoft.net/openal-releases/$(notdir $@)

$(CACHEDIR)/openal-soft-android-%.tar.gz:
	wget -nc -P $(CACHEDIR) https://github.com/kramlat/openal-soft-android/archive/v$*.tar.gz -O "$@"

$(BUILDDIR)/ogg: $(CACHEDIR)/libogg-1.3.4.tar.xz
	@mkdir -p "$@"
	tar xvfa "$<" --strip 1 --directory "$@"
	if [ -e $(ROOTDIR)/patches/$(notdir $@)/series ]; then cd "$@" && QUILT_PATCHES=$(ROOTDIR)/patches/$(notdir $@) quilt push -a; fi

$(BUILDDIR)/vorbis: $(CACHEDIR)/libvorbis-1.3.7.tar.xz
	@mkdir -p "$@"
	tar xvfa "$<" --strip 1 --directory "$@"
	if [ -e $(ROOTDIR)/patches/$(notdir $@)/series ]; then cd "$@" && QUILT_PATCHES=$(ROOTDIR)/patches/$(notdir $@) quilt push -a; fi

$(BUILDDIR)/flac: $(CACHEDIR)/flac-1.3.3.tar.xz
	@mkdir -p "$@"
	tar xvfa "$<" --strip 1 --directory "$@"
	if [ -e $(ROOTDIR)/patches/$(notdir $@)/series ]; then cd "$@" && QUILT_PATCHES=$(ROOTDIR)/patches/$(notdir $@) quilt push -a; fi

$(BUILDDIR)/freetype: $(CACHEDIR)/freetype-2.10.2.tar.xz
	@mkdir -p "$@"
	tar xvfa "$<" --strip 1 --directory "$@"
	if [ -e $(ROOTDIR)/patches/$(notdir $@)/series ]; then cd "$@" && QUILT_PATCHES=$(ROOTDIR)/patches/$(notdir $@) quilt push -a; fi

#~ $(BUILDDIR)/openal: $(CACHEDIR)/openal-soft-1.20.1.tar.bz2
$(BUILDDIR)/openal: $(CACHEDIR)/openal-soft-android-1.13.tar.gz
	@mkdir -p "$@"
	tar xvfa "$<" --strip 1 --directory "$@"
	if [ -e $(ROOTDIR)/patches/$(notdir $@)/series ]; then cd "$@" && QUILT_PATCHES=$(ROOTDIR)/patches/$(notdir $@) quilt push -a; fi

$(INSTALLDIR)/%/lib/libogg.a: $(BUILDDIR)/ogg
	mkdir -p "$<_$*"
	cd "$<_$*" && PATH="$(NDK_TOOLCHAINS)/bin/:$$PATH" \
		CC="$(CC_$*)" CXX="$(CXX_$*)" AR="$(NDK_BIN_$*)/ar" \
		CFLAGS="$(CFLAGS) -I '$(ROOTDIR)/$(INSTALLDIR)/$*/include/'" \
		CXXFLAGS="$(CXXFLAGS) -I '$(ROOTDIR)/$(INSTALLDIR)/$*/include/' -fno-rtti" \
		LDFLAGS="$(LDFLAGS) -L'$(ROOTDIR)/$(INSTALLDIR)/$*/lib/'" \
		"$(ROOTDIR)/$</configure" \
			need_version=no \
			--enable-shared=no \
			--host=$(HOST_$*) \
			--with-sysroot="$(NDK_SYSROOT)" \
			--with-build-sysroot="$(NDK_SYSROOT)" \
			--prefix="/$*"
	cd "$<_$*" && make && make install DESTDIR="$(ROOTDIR)/$(INSTALLDIR)/"

$(INSTALLDIR)/%/lib/libvorbis.a: $(BUILDDIR)/vorbis $(INSTALLDIR)/%/lib/libogg.a
	mkdir -p "$<_$*"
	cd "$<_$*" && PATH="$(NDK_TOOLCHAINS)/bin/:$$PATH" \
		CC="$(CC_$*)" CXX="$(CXX_$*)" AR="$(NDK_BIN_$*)/ar" \
		CFLAGS="$(CFLAGS) -I '$(ROOTDIR)/$(INSTALLDIR)/$*/include/'" \
		CXXFLAGS="$(CXXFLAGS) -I '$(ROOTDIR)/$(INSTALLDIR)/$*/include/' -fno-rtti" \
		LDFLAGS="$(LDFLAGS) -L'$(ROOTDIR)/$(INSTALLDIR)/$*/lib/'" \
		"$(ROOTDIR)/$</configure" \
			need_version=no \
			--enable-shared=no \
			--host=$(HOST_$*) \
			--with-sysroot="$(NDK_SYSROOT)" \
			--with-build-sysroot="$(NDK_SYSROOT)" \
			--prefix="/$*" \
			--with-ogg="$(ROOTDIR)/$(INSTALLDIR)/$*/"
	cd "$<_$*" && make && make install DESTDIR="$(ROOTDIR)/$(INSTALLDIR)/"

$(INSTALLDIR)/%/lib/libflac.a: $(BUILDDIR)/flac $(INSTALLDIR)/%/lib/libogg.a
	mkdir -p "$<_$*"
	cd "$<_$*" && PATH="$(NDK_TOOLCHAINS)/bin/:$$PATH" \
		CC="$(CC_$*)" CXX="$(CXX_$*)" AR="$(NDK_BIN_$*)/ar" \
		CFLAGS="$(CFLAGS) -I '$(ROOTDIR)/$(INSTALLDIR)/$*/include/'" \
		CXXFLAGS="$(CXXFLAGS) -I '$(ROOTDIR)/$(INSTALLDIR)/$*/include/'" \
		LDFLAGS="$(LDFLAGS) -L'$(ROOTDIR)/$(INSTALLDIR)/$*/lib/'" \
		"$(ROOTDIR)/$</configure" \
			need_version=no \
			--enable-shared=no \
			--host=$(HOST_$*) \
			--with-sysroot="$(NDK_SYSROOT)" \
			--with-build-sysroot="$(NDK_SYSROOT)" \
			--prefix="/$*" \
			--with-ogg="$(ROOTDIR)/$(INSTALLDIR)/$*/"
	cd "$<_$*" && make && make install DESTDIR="$(ROOTDIR)/$(INSTALLDIR)/"

$(INSTALLDIR)/%/lib/libfreetype.a: $(BUILDDIR)/freetype
	mkdir -p "$<_$*"
	cd "$<_$*" && PATH="$(NDK_TOOLCHAINS)/bin/:$$PATH" \
		CC="$(CC_$*)" CXX="$(CXX_$*)" AR="$(NDK_BIN_$*)/ar" \
		CFLAGS="$(CFLAGS) -I '$(ROOTDIR)/$(INSTALLDIR)/$*/include/'" \
		CXXFLAGS="$(CXXFLAGS) -I '$(ROOTDIR)/$(INSTALLDIR)/$*/include/' -fno-rtti" \
		LDFLAGS="$(LDFLAGS) -L'$(ROOTDIR)/$(INSTALLDIR)/$*/lib/'" \
		"$(ROOTDIR)/$</configure" \
			need_version=no \
			--with-brotli=no \
			--enable-shared=no \
			--host=$(HOST_$*) \
			--with-sysroot="$(NDK_SYSROOT)" \
			--with-build-sysroot="$(NDK_SYSROOT)" \
			--prefix="/$*"
	cd "$<_$*" && make && make install DESTDIR="$(ROOTDIR)/$(INSTALLDIR)/"

# https://kcat.strangesoft.net/openal.html
# http://github.com/kcat/openal-soft
# http://repo.or.cz/w/openal-soft.git
#~ $(INSTALLDIR)/%/lib/libopenal.a: $(BUILDDIR)/openal
#~ 	mkdir -p "$<_$*"
#~ 	cd "$<_$*" && PATH="$(NDK_TOOLCHAINS)/bin/:$$PATH" PKG_CONFIG_LIBDIR="$(INSTALLDIR)/$*/lib/pkgconfig" PKG_CONFIG_PATH="" cmake \
#~ 		-DCMAKE_BUILD_TYPE=Release \
#~ 		-DCMAKE_C_COMPILER="$(CC_$*)" \
#~ 		-DCMAKE_CXX_COMPILER="$(CXX_$*)" \
#~ 		-DALSOFT_REQUIRE_OPENSL=ON \
#~ 		-DALSOFT_EMBED_HRTF_DATA=YES \
#~ 		-DALSOFT_NO_CONFIG_UTIL=ON \
#~ 		-DALSOFT_EXAMPLES=OFF \
#~ 		-DALSOFT_UTILS=OFF \
#~ 		-DALSOFT_TESTS=OFF \
#~ 		-DOPENSL_LIBRARY="$(NDK_LIBS_$*)/$(ANDROID_API)" \
#~ 		-DCMAKE_TOOLCHAIN_FILE="$(NDK)/build/cmake/android.toolchain.cmake" \
#~ 		-DCMAKE_FIND_ROOT_PATH="$(NDK_SYSROOT)" \
#~ 		-DCMAKE_FIND_ROOT_PATH_MODE_PROGRAM=NEVER \
#~ 		-DCMAKE_FIND_ROOT_PATH_MODE_LIBRARY=ONLY \
#~ 		-DCMAKE_FIND_ROOT_PATH_MODE_INCLUDE=ONLY \
#~ 		-DCMAKE_FIND_ROOT_PATH_MODE_PACKAGE=ONLY \
#~ 		-DCMAKE_SYSROOT:STRING="$(NDK_SYSROOT)" \
#~ 		-DCMAKE_C_FLAGS:STRING="$(CFLAGS) -I '$(NDK_SYSROOT)/usr/include/' -I '$(ROOTDIR)/$(INSTALLDIR)/$*/include/'" \
#~ 		-DCMAKE_CXX_FLAGS:STRING="$(CXXFLAGS) -I '$(NDK_SYSROOT)/usr/include/' -I '$(ROOTDIR)/$(INSTALLDIR)/$*/include/'" \
#~ 		-DCMAKE_SHARED_LINKER_FLAGS:STRING="$(LDFLAGS) -L'$(ROOTDIR)/$(INSTALLDIR)/$*/lib/' -lOpenSLES" \
#~ 		-DCMAKE_EXE_LINKER_FLAGS:STRING="$(LDFLAGS) -L'$(ROOTDIR)/$(INSTALLDIR)/$*/lib/' -lOpenSLES" \
#~ 		-DCMAKE_INSTALL_PREFIX="/$*" \
#~ 		"$(ROOTDIR)/$<"
#~ 	cd "$<_$*" && make && make install DESTDIR="$(ROOTDIR)/$(INSTALLDIR)/"

# http://pielot.org/2010/12/openal-on-android/
# https://github.com/kramlat/openal-soft-android
# https://github.com/AerialX/openal-soft-android
# https://repo.or.cz/w/openal-soft/android.git
$(INSTALLDIR)/%/lib/libopenal.a: $(BUILDDIR)/openal
	mkdir -p "$<_$*"
	cd "$<_$*" && PATH="$(NDK_TOOLCHAINS)/bin/:$$PATH" PKG_CONFIG_LIBDIR="$(INSTALLDIR)/$*/lib/pkgconfig" PKG_CONFIG_PATH="" cmake \
		-DCMAKE_C_COMPILER="$(CC_$*)" -DCMAKE_CXX_COMPILER="$(CXX_$*)" CMAKE_AR="$(NDK_BIN_$*)/ar" \
		-DALSOFT_REQUIRE_OPENSL=ON \
		-DANDROID_ABI=$(ANDROID_ABI) \
		-DANDROID_NATIVE_API_LEVEL=android-9 \
		-DALSOFT_ANDROID_LOW_LATENCY:BOOL=ON \
		-DALSOFT_UTILS=ON \
		-DALSOFT_EXAMPLES=OFF \
		-DALSOFT_UTILS=OFF \
		-DCMAKE_TOOLCHAIN_FILE="$(NDK)/build/cmake/android.toolchain.cmake" \
		-DCMAKE_FIND_ROOT_PATH="$(NDK_SYSROOT)" \
		-DCMAKE_FIND_ROOT_PATH_MODE_PROGRAM=NEVER \
		-DCMAKE_FIND_ROOT_PATH_MODE_LIBRARY=ONLY \
		-DCMAKE_FIND_ROOT_PATH_MODE_INCLUDE=ONLY \
		-DCMAKE_FIND_ROOT_PATH_MODE_PACKAGE=ONLY \
		-DCMAKE_SYSROOT="$(NDK_SYSROOT)" \
		-DCMAKE_C_FLAGS="$(CFLAGS) -I '$(NDK_SYSROOT)/usr/include/' -I '$(ROOTDIR)/$(INSTALLDIR)/$*/include/'" \
		-DCMAKE_CXX_FLAGS="$(CXXFLAGS) -I '$(NDK_SYSROOT)/usr/include/' -I '$(ROOTDIR)/$(INSTALLDIR)/$*/include/'" \
		-DCMAKE_SHARED_LINKER_FLAGS="$(LDFLAGS) -L'$(ROOTDIR)/$(INSTALLDIR)/$*/lib/'" \
		-DCMAKE_EXE_LINKER_FLAGS="$(LDFLAGS) -L'$(ROOTDIR)/$(INSTALLDIR)/$*/lib/'" \
		-DCMAKE_INSTALL_PREFIX="/$*" \
		-DCMAKE_BUILD_TYPE=Release \
		-DLIBTYPE=STATIC \
		"$(ROOTDIR)/$<"
	sed -i 's|usr/local|$*|g' "$<_$*/CMakeCache.txt" # Hack due to https://cmake.org/pipermail/cmake/2011-February/042629.html
	cd "$<_$*" && make && make install DESTDIR="$(ROOTDIR)/$(INSTALLDIR)/"

.PHONY: build-libs
build-libs: build-libs/armv7a build-libs/aarch64 build-libs/i686 build-libs/x86_64
	@#

.PHONY: build-libs/%
build-libs/%: $(INSTALLDIR)/%/lib/libogg.a $(INSTALLDIR)/%/lib/libvorbis.a $(INSTALLDIR)/%/lib/libflac.a $(INSTALLDIR)/%/lib/libfreetype.a
	@#

.PHONY: install-libs/%
install-libs/%: build-libs/%
	mkdir -p "$(DEPLOYDIR)/$*/include"
	( cd "$(INSTALLDIR)/$*/include" ; tar cf - . ) | ( cd "$(DEPLOYDIR)/$*/include" ; tar xf - )
	mkdir -p "$(DEPLOYDIR)/$*/bin"
	( cd "$(INSTALLDIR)/$*/bin" ; tar cf - . ) | ( cd "$(DEPLOYDIR)/$*/bin" ; tar xf - )
	mkdir -p "$(DEPLOYDIR)/$*/lib"
	( cd "$(INSTALLDIR)/$*/lib" ; tar cf - . ) | ( cd "$(DEPLOYDIR)/$*/lib" ; tar xf - )

	find "$(DEPLOYDIR)/$*/bin" -type f -executable | while read -r FILE; do "$(NDK_BIN_$*)/strip" "$$FILE"; done
	find "$(DEPLOYDIR)/$*/lib" -type f -name "*.a" | while read -r FILE; do "$(NDK_BIN_$*)/strip" "$$FILE"; done
	find "$(DEPLOYDIR)/$*/lib" -type f -name "*.so*" | while read -r FILE; do "$(NDK_BIN_$*)/strip" "$$FILE"; done

.PHONY: install-libs
install-libs: install-libs/armv7a install-libs/aarch64 install-libs/i686 install-libs/x86_64

.PHONY: clean
clean:
	if [ -n "$(BUILDDIR)" -a -d "$(BUILDDIR)" ]; then rm -rf "$(BUILDDIR)"; fi
	if [ -n "$(INSTALLDIR)" -a -d "$(INSTALLDIR)" ]; then rm -rf "$(INSTALLDIR)"; fi

DEFS :=
LIBS :=

$(BUILDDIR)/obj/armv7a/%.o: src/%.c
	@mkdir -p $(dir $@)
	$(CC_$(notdir $(dir $@))) $(DEFS) $(CFLAGS) -c -o "$@" "$<"

$(BUILDDIR)/obj/armv7a/%.o: src/%.cpp
	@mkdir -p $(dir $@)
	$(CXX_$(notdir $(dir $@))) $(DEFS) $(CXXFLAGS) -c -o "$@" "$<"

$(BUILDDIR)/apk/lib/armv7a/lib%.so:
	@mkdir -p $(dir $@)
	$(CXX_$(notdir $(dir $@))) -shared -o "$@" $+ $(LDFLAGS) $(LIBS)
