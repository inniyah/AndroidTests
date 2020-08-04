all: build-libs install-libs

ROOTDIR := $(dir $(abspath $(lastword $(MAKEFILE_LIST))))
CACHEDIR := cache
BUILDDIR := build
INSTALLDIR := build/output
DEPLOYDIR := .
TMPDIR := build/tmp

COMMON_CFLAGS = -O2 -gz -g3 -fPIC -Wall -Wdate-time -D_FORTIFY_SOURCE=2 -D_GNU_SOURCE \
	-Werror=int-conversion -Werror=implicit-function-declaration
CFLAGS = -std=c11 $(COMMON_CFLAGS)
CXXFLAGS = -std=c++11 $(COMMON_CFLAGS)
LDFLAGS = -Wl,--as-needed -Wl,--no-undefined -Wl,--no-allow-shlib-undefined

ANDROID_API = 23
ANDROID_NATIVE_API = 18

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

ANDROID_ABI_armv7a = armeabi-v7a
ANDROID_ABI_aarch64 = arm64-v8a
ANDROID_ABI_i686 = x86
ANDROID_ABI_x86_64 = x86_64
ANDROID_ABI_mipsel = mipsel
ANDROID_ABI_mips64el = mips64el

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

NDK_INCS_armv7a = $(NDK_SYSROOT)/usr/include/arm-linux-androideabi
NDK_INCS_aarch64 = $(NDK_SYSROOT)/usr/include/aarch64-linux-android
NDK_INCS_i686 = $(NDK_SYSROOT)/usr/include/i686-linux-android
NDK_INCS_x86_64 = $(NDK_SYSROOT)/usr/include/x86_64-linux-android
NDK_INCS_mipsel = $(NDK_SYSROOT)/usr/include/mipsel-linux-android
NDK_INCS_mips64el = $(NDK_SYSROOT)/usr/include/mips64el-linux-android

check:
	@command -v wget  >/dev/null || ( >&2 echo "Package 'wget' is needed"     ; false )
	@command -v quilt >/dev/null || ( >&2 echo "Package 'quilt' is needed"    ; false )
	@command -v unzip >/dev/null || ( >&2 echo "Package 'unzip' is needed"    ; false )
	@command -v tar   >/dev/null || ( >&2 echo "Package 'tar' is needed"      ; false )
	@command -v gzip  >/dev/null || ( >&2 echo "Package 'gzip' is needed"     ; false )
	@command -v bzip2 >/dev/null || ( >&2 echo "Package 'bzip2' is needed"    ; false )
	@command -v xz    >/dev/null || ( >&2 echo "Package 'xz-utils' is needed" ; false )
	@test -n "$(JDK)" -a -d "$(JDK)"                         || ( >&2 echo "Package 'openjdk-8-jdk' is needed" ; false )
	@test -n "$(SDK)" -a -d "$(SDK)"                         || ( >&2 echo "Package 'android-sdk' is needed" ; false )
	@test -n "$(NDK)" -a -d "$(NDK)"                         || ( >&2 echo "Package 'google-android-ndk-installer' is needed" ; false )
	@test -n "$(JAVAC)" -o ! -x "$(JAVAC)"                   || ( >&2 echo "Package 'openjdk-8-jdk' is needed" ; false )
	@test -n "$(JAVAH)" -o ! -x "$(JAVAH)"                   || ( >&2 echo "Package 'openjdk-8-jdk' is needed" ; false )
	@test -n "$(KEYTOOL)" -o ! -x "$(KEYTOOL)"               || ( >&2 echo "Package 'openjdk-8-jre' is needed" ; false )
	@test -n "$(BUILD_TOOLS_DIR)" -a -d "$(BUILD_TOOLS_DIR)" || ( >&2 echo "Package 'android-sdk' is needed" ; false )
	@test -n "$(AAPT)" -o ! -x "$(AAPT)"                     || ( >&2 echo "Package 'aapt' is needed" ; false )
	@test -n "$(DX)" -o ! -x "$(DX)"                         || ( >&2 echo "Package 'dalvik-exchange' is needed" ; false )
	@test -n "$(ZIPALIGN)" -o ! -x "$(ZIPALIGN)"             || ( >&2 echo "Package 'zipalign' is needed" ; false )
	@test -n "$(APKSIGNER)" -o ! -x "$(APKSIGNER)"           || ( >&2 echo "Package 'apksigner' is needed" ; false )
	@test -n "$(ADB)" -o ! -x "$(ADB)"                       || ( >&2 echo "Package 'adb' is needed" ; false )
	@test -n "$(PLATFORM)" -o ! -r "$(PLATFORM)"             || ( >&2 echo "Package 'libandroid-$(ANDROID_API)-java' is needed" ; false )
	@test -n "$(NDK_SYSROOT)" -a -d "$(NDK_SYSROOT)"         || ( >&2 echo "Package 'google-android-ndk-installer' is needed" ; false )

$(CACHEDIR)/flac-1.3.3.tar.xz:
	@mkdir -p $(CACHEDIR)
	wget -nc -P $(CACHEDIR) "http://downloads.xiph.org/releases/flac/$(notdir $@)"

$(CACHEDIR)/libvorbis-1.3.7.tar.xz:
	@mkdir -p $(CACHEDIR)
	wget -nc -P $(CACHEDIR) "https://ftp.osuosl.org/pub/xiph/releases/vorbis/$(notdir $@)"

$(CACHEDIR)/libogg-1.3.4.tar.xz:
	@mkdir -p $(CACHEDIR)
	wget -nc -P $(CACHEDIR) "https://ftp.osuosl.org/pub/xiph/releases/ogg/$(notdir $@)"

$(CACHEDIR)/freetype-2.10.2.tar.xz:
	@mkdir -p $(CACHEDIR)
	wget -nc -P $(CACHEDIR) "http://download.savannah.gnu.org/releases/freetype/$(notdir $@)"

$(CACHEDIR)/harfbuzz-%.tar.gz:
	wget -nc -P $(CACHEDIR) "https://github.com/harfbuzz/harfbuzz/archive/$*.tar.gz" -O "$@"

$(CACHEDIR)/openal-soft-1.20.1.tar.bz2:
	wget -nc -P $(CACHEDIR) "https://kcat.strangesoft.net/openal-releases/$(notdir $@)"

$(CACHEDIR)/openal-soft-android-%.tar.gz:
	wget -nc -P $(CACHEDIR) "https://github.com/kramlat/openal-soft-android/archive/v$*.tar.gz" -O "$@"

$(CACHEDIR)/sfml-%.tar.gz:
	wget -nc -P $(CACHEDIR) "https://github.com/SFML/SFML/archive/$*.tar.gz" -O "$@"

$(CACHEDIR)/stb-master.zip:
	wget -nc -P $(CACHEDIR) https://github.com/nothings/stb/archive/master.zip -O "$@"

$(CACHEDIR)/libpng-%.tar.xz:
	wget -nc -P $(CACHEDIR) "https://download.sourceforge.net/libpng/libpng-$*.tar.xz" -O "$@"

$(CACHEDIR)/raylib-%.tar.gz:
	wget -nc -P $(CACHEDIR) "https://github.com/raysan5/raylib/archive/$*.tar.gz" -O "$@"

$(CACHEDIR)/raylib-%.tar.gz:
	wget -nc -P $(CACHEDIR) "https://github.com/raysan5/raylib/archive/$*.tar.gz" -O "$@"

$(CACHEDIR)/glfw3-%.tar.gz:
	wget -nc -P $(CACHEDIR) "https://github.com/inniyah/glfw/archive/android_$*.tar.gz" -O "$@"

#~ $(CACHEDIR)/glfm-%.tar.gz:
#~ 	wget -nc -P $(CACHEDIR) "https://github.com/brackeen/glfm/archive/$*.tar.gz" -O "$@"

$(CACHEDIR)/glfm-master.zip:
	wget -nc -P $(CACHEDIR) https://github.com/brackeen/glfm/archive/master.zip -O "$@"

$(CACHEDIR)/nanovg-%.tar.gz:
	wget -nc -P $(CACHEDIR) "https://github.com/inniyah/nanovg/archive/v$*.tar.gz" -O "$@"

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

$(BUILDDIR)/harfbuzz: $(CACHEDIR)/harfbuzz-2.7.0.tar.gz
	@mkdir -p "$@"
	tar xvfa "$<" --strip 1 --directory "$@"
	if [ -e $(ROOTDIR)/patches/$(notdir $@)/series ]; then cd "$@" && QUILT_PATCHES=$(ROOTDIR)/patches/$(notdir $@) quilt push -a; fi

#~ $(BUILDDIR)/openal: $(CACHEDIR)/openal-soft-1.20.1.tar.bz2
$(BUILDDIR)/openal: $(CACHEDIR)/openal-soft-android-1.13.tar.gz
	@mkdir -p "$@"
	tar xvfa "$<" --strip 1 --directory "$@"
	if [ -e $(ROOTDIR)/patches/$(notdir $@)/series ]; then cd "$@" && QUILT_PATCHES=$(ROOTDIR)/patches/$(notdir $@) quilt push -a; fi

$(BUILDDIR)/stb: $(CACHEDIR)/stb-master.zip
	@mkdir -p "$@"
	cd "$@" && ln -s . $(notdir $@)-master
	unzip "$<" -d "$@"
	rm "$@/$(notdir $@)-master"
	if [ -e $(ROOTDIR)/patches/$(notdir $@)/series ]; then cd "$@" && QUILT_PATCHES=$(ROOTDIR)/patches/$(notdir $@) quilt push -a; fi

$(BUILDDIR)/png: $(CACHEDIR)/libpng-1.6.37.tar.xz
	@mkdir -p "$@"
	tar xvfa "$<" --strip 1 --directory "$@"
	if [ -e $(ROOTDIR)/patches/$(notdir $@)/series ]; then cd "$@" && QUILT_PATCHES=$(ROOTDIR)/patches/$(notdir $@) quilt push -a; fi

$(BUILDDIR)/sfml: $(CACHEDIR)/sfml-2.5.1.tar.gz
	@mkdir -p "$@"
	tar xvfa "$<" --strip 1 --directory "$@"
	rm -rf "$@/extlibs"
	if [ -e $(ROOTDIR)/patches/$(notdir $@)/series ]; then cd "$@" && QUILT_PATCHES=$(ROOTDIR)/patches/$(notdir $@) quilt push -a; fi

$(BUILDDIR)/raylib: $(CACHEDIR)/raylib-3.0.0.tar.gz
	@mkdir -p "$@"
	tar xvfa "$<" --strip 1 --directory "$@"
	if [ -e $(ROOTDIR)/patches/$(notdir $@)/series ]; then cd "$@" && QUILT_PATCHES=$(ROOTDIR)/patches/$(notdir $@) quilt push -a; fi

$(BUILDDIR)/glfw3: $(CACHEDIR)/glfw3-3.3+1.tar.gz
	@mkdir -p "$@"
	tar xvfa "$<" --strip 1 --directory "$@"
	if [ -e $(ROOTDIR)/patches/$(notdir $@)/series ]; then cd "$@" && QUILT_PATCHES=$(ROOTDIR)/patches/$(notdir $@) quilt push -a; fi

#~ $(BUILDDIR)/glfm: $(CACHEDIR)/glfm-0.9.0.tar.gz
#~ 	@mkdir -p "$@"
#~ 	tar xvfa "$<" --strip 1 --directory "$@"
#~ 	if [ -e $(ROOTDIR)/patches/$(notdir $@)/series ]; then cd "$@" && QUILT_PATCHES=$(ROOTDIR)/patches/$(notdir $@) quilt push -a; fi

$(BUILDDIR)/glfm: $(CACHEDIR)/glfm-master.zip
	@mkdir -p "$@"
	cd "$@" && ln -s . $(notdir $@)-master
	unzip "$<" -d "$@"
	rm "$@/$(notdir $@)-master"
	if [ -e $(ROOTDIR)/patches/$(notdir $@)/series ]; then cd "$@" && QUILT_PATCHES=$(ROOTDIR)/patches/$(notdir $@) quilt push -a; fi

$(BUILDDIR)/nanovg: $(CACHEDIR)/nanovg-0.1-rc2.tar.gz
	@mkdir -p "$@"
	tar xvfa "$<" --strip 1 --directory "$@"
	if [ -e $(ROOTDIR)/patches/$(notdir $@)/series ]; then cd "$@" && QUILT_PATCHES=$(ROOTDIR)/patches/$(notdir $@) quilt push -a; fi

# Ogg bitstream library
# http://xiph.org/ogg/

.PHONY: build-ogg/%
.PRECIOUS: $(INSTALLDIR)/%/lib/libogg.a
build-ogg/% $(INSTALLDIR)/%/lib/libogg.a: $(BUILDDIR)/ogg
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

# Decoder library for Vorbis General Audio Compression Codec
# https://www.xiph.org/vorbis/

.PHONY: build-vorbis/%
.PRECIOUS: $(INSTALLDIR)/%/lib/libvorbis.a
.PRECIOUS: $(INSTALLDIR)/%/lib/libvorbisfile.a
.PRECIOUS: $(INSTALLDIR)/%/lib/libvorbisenc.a
build-vorbis/% $(INSTALLDIR)/%/lib/libvorbisfile.a $(INSTALLDIR)/%/lib/libvorbisenc.a \
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

# Free Lossless Audio Codec
# http://xiph.org/flac/

.PHONY: build-flac/%
.PRECIOUS: $(INSTALLDIR)/%/lib/libFLAC.a
build-flac/% $(INSTALLDIR)/%/lib/libFLAC.a: $(BUILDDIR)/flac $(INSTALLDIR)/%/lib/libogg.a
	mkdir -p "$<_$*"
	cd "$<_$*" && PATH="$(NDK_TOOLCHAINS)/bin/:$$PATH" \
		CC="$(CC_$*)" CXX="$(CXX_$*)" AR="$(NDK_BIN_$*)/ar" \
		CFLAGS="$(CFLAGS) -I '$(ROOTDIR)/$(INSTALLDIR)/$*/include/' -DFLAC__NO_DLL" \
		CXXFLAGS="$(CXXFLAGS) -I '$(ROOTDIR)/$(INSTALLDIR)/$*/include/' -DFLAC__NO_DLL" \
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

# FreeType 2 font engine
# https://www.freetype.org/

.PHONY: build-freetype/%
.PRECIOUS:$(INSTALLDIR)/%/lib/libfreetype.a
build-freetype/% $(INSTALLDIR)/%/lib/libfreetype.a: $(BUILDDIR)/freetype
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
	mkdir -p "$(ROOTDIR)/$(INSTALLDIR)/$*/lib/"
	install -m 644 "$<_$*/.libs"/lib*.a "$(ROOTDIR)/$(INSTALLDIR)/$*/lib/"

# OpenType text shaping engine
# https://www.freedesktop.org/wiki/Software/HarfBuzz

.PHONY: build-harfbuzz/%
.PRECIOUS: $(INSTALLDIR)/%/lib/libharfbuzz.a
build-harfbuzz/% $(INSTALLDIR)/%/lib/libharfbuzz.a: $(BUILDDIR)/harfbuzz $(INSTALLDIR)/%/lib/libfreetype.a
	mkdir -p "$<_$*"
	cd "$<_$*" && cmake \
		-DCMAKE_BUILD_TYPE=Release \
		-DHB_HAVE_FREETYPE=ON \
		-DFREETYPE_LIBRARY="'$(ROOTDIR)/$(INSTALLDIR)/$*/lib/" \
		-DFREETYPE_INCLUDE_DIRS="$(ROOTDIR)/$(INSTALLDIR)/$*/include/freetype2/" \
		-DANDROID_ABI=$(ANDROID_ABI_$*) \
		-DANDROID_NATIVE_API_LEVEL=android-$(ANDROID_NATIVE_API) \
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
		"$(ROOTDIR)/$<"
	cd "$<_$*" && make && make install DESTDIR="$(ROOTDIR)/$(INSTALLDIR)/"

# Open Audio Library
# http://kcat.strangesoft.net/openal.html

# https://kcat.strangesoft.net/openal.html
# http://github.com/kcat/openal-soft
# http://repo.or.cz/w/openal-soft.git
#~ .PHONY: build-openal/%
#~ .PRECIOUS: $(INSTALLDIR)/%/lib/libopenal.a
#~ .PRECIOUS: $(INSTALLDIR)/%/lib/libopenal.so
#~ $(INSTALLDIR)/%/lib/libopenal.so: $(BUILDDIR)/openal
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
.PHONY: build-openal/%
.PRECIOUS: $(INSTALLDIR)/%/lib/libopenal.a
.PRECIOUS: $(INSTALLDIR)/%/lib/libopenal.so
build-openal/% $(INSTALLDIR)/%/lib/libopenal.so: $(BUILDDIR)/openal
	mkdir -p "$<_$*"
	cd "$<_$*" && cmake \
		-DCMAKE_BUILD_TYPE=Release \
		-DALSOFT_REQUIRE_OPENSL=ON \
		-DANDROID_ABI=$(ANDROID_ABI_$*) \
		-DANDROID_NATIVE_API_LEVEL=android-$(ANDROID_NATIVE_API) \
		-DALSOFT_ANDROID_LOW_LATENCY:BOOL=ON \
		-DALSOFT_UTILS=ON \
		-DALSOFT_EXAMPLES=OFF \
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
		-DLIBTYPE=SHARED \
		"$(ROOTDIR)/$<"
	cd "$<_$*" && make && make install DESTDIR="$(ROOTDIR)/$(INSTALLDIR)/"

# Single-file public domain (or MIT licensed) libraries
# https://github.com/nothings/stb

.PHONY: build-stb/%
.PRECIOUS: $(INSTALLDIR)/%/include/stb
build-stb/% $(INSTALLDIR)/%/include/stb: $(BUILDDIR)/stb
	mkdir -p "$(INSTALLDIR)/$*/include/stb"
	install -m 644 "$<"/*.h "$(INSTALLDIR)/$*/include/stb"

# Library for reading and writing PNG (Portable Network Graphics) format files
# http://libpng.org/pub/png/libpng.html

.PHONY: build-png/%
.PRECIOUS: $(INSTALLDIR)/%/lib/libpng16.a
build-png/% $(INSTALLDIR)/%/lib/libpng16.a: $(BUILDDIR)/png
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

# Simple and Fast Multimedia Library
# http://www.sfml-dev.org/

.PHONY: build-sfml/%
.PRECIOUS: $(INSTALLDIR)/%/lib/libsfml-activity.so
.PRECIOUS: $(INSTALLDIR)/%/lib/libsfml-audio.so
.PRECIOUS: $(INSTALLDIR)/%/lib/libsfml-graphics.so
.PRECIOUS: $(INSTALLDIR)/%/lib/libsfml-main.a
.PRECIOUS: $(INSTALLDIR)/%/lib/libsfml-network.so
.PRECIOUS: $(INSTALLDIR)/%/lib/libsfml-system.so
.PRECIOUS: $(INSTALLDIR)/%/lib/libsfml-window.so
build-sfml/% \
$(INSTALLDIR)/%/lib/libsfml-activity.so $(INSTALLDIR)/%/lib/libsfml-audio.so $(INSTALLDIR)/%/lib/libsfml-graphics.so \
$(INSTALLDIR)/%/lib/libsfml-main.a $(INSTALLDIR)/%/lib/libsfml-network.so \
$(INSTALLDIR)/%/lib/libsfml-system.so $(INSTALLDIR)/%/lib/libsfml-window.so: $(BUILDDIR)/sfml \
				$(INSTALLDIR)/%/lib/libogg.a $(INSTALLDIR)/%/lib/libvorbis.a $(INSTALLDIR)/%/lib/libFLAC.a \
				$(INSTALLDIR)/%/lib/libfreetype.a $(INSTALLDIR)/%/lib/libpng16.a $(INSTALLDIR)/%/lib/libharfbuzz.a \
				$(INSTALLDIR)/%/include/stb $(INSTALLDIR)/%/lib/libopenal.so
	mkdir -p "$<_$*"
	cd "$<_$*" && cmake \
		--debug-output \
		-DCMAKE_VERBOSE_MAKEFILE=ON \
		-DCMAKE_BUILD_TYPE=Release \
		-DCMAKE_SYSTEM_NAME=Android \
		-DANDROID_PLATFORM=android-$(ANDROID_API) \
		-DCMAKE_ANDROID_NDK="$(NDK)" \
		-DANDROID_ABI=$(ANDROID_ABI_$*) \
		-DCMAKE_ANDROID_API=$(ANDROID_API) \
		-DANDROID_NATIVE_API_LEVEL=android-$(ANDROID_NATIVE_API) \
		-DANDROID_STL=c++_shared \
		-DCMAKE_TOOLCHAIN_FILE="$(NDK)/build/cmake/android.toolchain.cmake" \
		-DCMAKE_FIND_ROOT_PATH="$(NDK_SYSROOT)" \
		-DCMAKE_FIND_ROOT_PATH_MODE_PROGRAM=NEVER \
		-DCMAKE_FIND_ROOT_PATH_MODE_LIBRARY=ONLY \
		-DCMAKE_FIND_ROOT_PATH_MODE_INCLUDE=ONLY \
		-DCMAKE_FIND_ROOT_PATH_MODE_PACKAGE=ONLY \
		-DCMAKE_SYSROOT="$(NDK_SYSROOT)" \
		-DCMAKE_C_FLAGS="$(CFLAGS) -I '$(NDK_SYSROOT)/usr/include/' -I '$(ROOTDIR)/$(INSTALLDIR)/$*/include/' -I '$(ROOTDIR)/$(INSTALLDIR)/$*/include/freetype2/' -I '$(ROOTDIR)/$(INSTALLDIR)/$*/include/stb/' -I '$(ROOTDIR)/$(INSTALLDIR)/$*/include/AL/'" \
		-DCMAKE_CXX_FLAGS="$(CXXFLAGS) -I '$(NDK_SYSROOT)/usr/include/' -I '$(ROOTDIR)/$(INSTALLDIR)/$*/include/' -I '$(ROOTDIR)/$(INSTALLDIR)/$*/include/freetype2/' -I '$(ROOTDIR)/$(INSTALLDIR)/$*/include/stb/' -I '$(ROOTDIR)/$(INSTALLDIR)/$*/include/AL/'" \
		-DCMAKE_MODULE_LINKER_FLAGS="$(LDFLAGS) -L'$(ROOTDIR)/$(INSTALLDIR)/$*/lib/' -llog" \
		-DCMAKE_SHARED_LINKER_FLAGS="$(LDFLAGS) -L'$(ROOTDIR)/$(INSTALLDIR)/$*/lib/' -llog" \
		-DCMAKE_EXE_LINKER_FLAGS="$(LDFLAGS)    -L'$(ROOTDIR)/$(INSTALLDIR)/$*/lib/' -llog" \
		-DCMAKE_INSTALL_PREFIX="/$*" \
		"$(ROOTDIR)/$<"
	cd "$<_$*" && make && make install DESTDIR="$(ROOTDIR)/$(TMPDIR)/"
	( cd "$(TMPDIR)/usr/lib/android-ndk/sources/third_party/sfml/include" ; tar cf - . ) | ( cd "$(INSTALLDIR)/$*/include" ; tar xf - )
	( cd "$(TMPDIR)/usr/lib/android-ndk/sources/third_party/sfml/lib/$(ANDROID_ABI_$*)" ; tar cf - . ) | ( cd "$(INSTALLDIR)/$*/lib" ; tar xf - )

# Simple and easy-to-use library to enjoy videogames programming
# https://www.raylib.com/

RAYLIB_ARCH_NAME_armv7a = ARM
RAYLIB_ARCH_NAME_aarch64 = ARM64
RAYLIB_ARCH_NAME_i686 = x86
RAYLIB_ARCH_NAME_x86_64 = x86_64

.PHONY: build-raylib/%
.PRECIOUS: $(INSTALLDIR)/%/lib/libraylib.a
build-raylib/% $(INSTALLDIR)/%/lib/libraylib.a: $(BUILDDIR)/raylib
	mkdir -p "$<_$*/src"
	mkdir -p "$(INSTALLDIR)/$*/bin"
	mkdir -p "$(INSTALLDIR)/$*/lib"
	mkdir -p "$(INSTALLDIR)/$*/include"
	( cd "$</src" ; tar cf - . ) | ( cd "$<_$*/src" ; tar xf - )
	cd "$<_$*/src" && PATH="$(NDK_TOOLCHAINS)/bin/:$$PATH" \
		CC="$(CC_$*)" CXX="$(CXX_$*)" AR="$(NDK_BIN_$*)/ar" \
		CFLAGS="$(CFLAGS) -I '$(ROOTDIR)/$(INSTALLDIR)/$*/include/' -I '$(NDK_INCS_$*)' -I '$(NDK_SYSROOT)/usr/include' -I '$(NDK)/sources/android/native_app_glue/'" \
		CXXFLAGS="$(CXXFLAGS) -I '$(ROOTDIR)/$(INSTALLDIR)/$*/include/' -I '$(NDK_INCS_$*)' -I '$(NDK_SYSROOT)/usr/include' -I '$(NDK)/sources/android/native_app_glue/' -fno-rtti" \
		LDFLAGS="$(LDFLAGS) -L'$(ROOTDIR)/$(INSTALLDIR)/$*/lib/'" \
		make RAYLIB_SRC_PATH="$(ROOTDIR)/$<_$*/src" RAYLIB_RELEASE_PATH="$(ROOTDIR)/$(INSTALLDIR)/$*/lib" \
			PLATFORM=PLATFORM_ANDROID ANDROID_NDK="$(NDK)" ANDROID_TOOLCHAIN="$(NDK_TOOLCHAINS)" ANDROID_ARCH="$(RAYLIB_ARCH_NAME_$*)" ANDROID_API_VERSION=$(ANDROID_API)
	install -m 644 "$<_$*/src/raylib.h" "$(INSTALLDIR)/$*/include"

# Open Source, multi-platform library for OpenGL, OpenGL ES and Vulkan development
# https://www.glfw.org/

.PHONY: build-glfw3/%
.PRECIOUS: $(INSTALLDIR)/%/lib/libglfw3.a
build-glfw3/% $(INSTALLDIR)/%/lib/libglfw3.a: $(BUILDDIR)/glfw3
	mkdir -p "$<_$*"
	cd "$<_$*" && cmake \
		-DCMAKE_BUILD_TYPE=Release \
		-DGLFW_BUILD_EXAMPLES=OFF \
		-DGLFW_BUILD_TESTS=OFF \
		-DGLFW_BUILD_DOCS=OFF \
		-DGLFW_INSTALL=ON \
		-DANDROID_ABI=$(ANDROID_ABI_$*) \
		-DANDROID_NATIVE_API_LEVEL=android-$(ANDROID_NATIVE_API) \
		-DCMAKE_TOOLCHAIN_FILE="$(NDK)/build/cmake/android.toolchain.cmake" \
		-DCMAKE_FIND_ROOT_PATH="$(NDK_SYSROOT)" \
		-DCMAKE_FIND_ROOT_PATH_MODE_PROGRAM=NEVER \
		-DCMAKE_FIND_ROOT_PATH_MODE_LIBRARY=ONLY \
		-DCMAKE_FIND_ROOT_PATH_MODE_INCLUDE=ONLY \
		-DCMAKE_FIND_ROOT_PATH_MODE_PACKAGE=ONLY \
		-DCMAKE_SYSROOT="$(NDK_SYSROOT)" \
		-DCMAKE_C_FLAGS="$(CFLAGS) -I '$(NDK_SYSROOT)/usr/include/' -I '$(ROOTDIR)/$(INSTALLDIR)/$*/include/'" \
		-DCMAKE_CXX_FLAGS="$(CXXFLAGS) -I '$(NDK_SYSROOT)/usr/include/' -I '$(ROOTDIR)/$(INSTALLDIR)/$*/include/'" \
		-DCMAKE_SHARED_LINKER_FLAGS="$(LDFLAGS) -landroid -L'$(ROOTDIR)/$(INSTALLDIR)/$*/lib/'" \
		-DCMAKE_EXE_LINKER_FLAGS="$(LDFLAGS) -landroid -L'$(ROOTDIR)/$(INSTALLDIR)/$*/lib/'" \
		-DCMAKE_INSTALL_PREFIX="/$*" \
		"$(ROOTDIR)/$<"
	mkdir -p "$(INSTALLDIR)/$*/bin"
	mkdir -p "$(INSTALLDIR)/$*/lib"
	mkdir -p "$(INSTALLDIR)/$*/include"
	cd "$<_$*" && make && make install DESTDIR="$(ROOTDIR)/$(INSTALLDIR)/"

# OpenGL ES layer for mobile devices and the web
# https://github.com/brackeen/glfm

.PHONY: build-glfm/%
.PRECIOUS: $(INSTALLDIR)/%/lib/libglfm.a
build-glfm/% $(INSTALLDIR)/%/lib/libglfm.a: $(BUILDDIR)/glfm
	mkdir -p "$<_$*"
	cd "$<_$*" && cmake \
		-DCMAKE_BUILD_TYPE=Release \
		-DANDROID_ABI=$(ANDROID_ABI_$*) \
		-DANDROID_NATIVE_API_LEVEL=android-$(ANDROID_NATIVE_API) \
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
		"$(ROOTDIR)/$<"
	cd "$<_$*" && make 
	mkdir -p "$(INSTALLDIR)/$*/bin"
	mkdir -p "$(INSTALLDIR)/$*/lib"
	mkdir -p "$(INSTALLDIR)/$*/include"
	install -m 644 "$<_$*"/*.a "$(INSTALLDIR)/$*/lib"
	install -m 644 "$</include"/*.h "$(INSTALLDIR)/$*/include"

# Small antialiased vector graphics rendering library for OpenGL
# https://github.com/memononen/nanovg

.PHONY: build-nanovg/%
.PRECIOUS: $(INSTALLDIR)/%/lib/libnanovg.a
build-nanovg/% $(INSTALLDIR)/%/lib/libnanovg.a: $(BUILDDIR)/nanovg $(INSTALLDIR)/%/include/stb
	mkdir -p "$<_$*"
	cd "$<_$*" && cmake \
		-DCMAKE_BUILD_TYPE=Release \
		-DNANOVG_SHARED_LIBS=OFF \
		-DNANOVG_STATIC_LIBS=ON \
		-DNANOVG_BUILD_GL2=OFF \
		-DNANOVG_BUILD_GL3=OFF \
		-DNANOVG_BUILD_EXAMPLES=OFF \
		-DANDROID_ABI=$(ANDROID_ABI_$*) \
		-DANDROID_NATIVE_API_LEVEL=android-$(ANDROID_NATIVE_API) \
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
		"$(ROOTDIR)/$<"
	cd "$<_$*" && make && make install DESTDIR="$(ROOTDIR)/$(INSTALLDIR)/"


.PHONY: build-libs
build-libs: check build-libs/armv7a build-libs/aarch64 build-libs/i686 build-libs/x86_64
	@#

.PHONY: install-libs
install-libs: check install-libs/armv7a install-libs/aarch64 install-libs/i686 install-libs/x86_64
	@#

.PHONY: install-libs-raw
install-libs-raw: install-libs-raw/armv7a install-libs-raw/aarch64 install-libs-raw/i686 install-libs-raw/x86_64
	@#

.PHONY: build-%
build-%: check build-%/armv7a build-%/aarch64 build-%/i686 build-%/x86_64
	@#

.PHONY: clean cleanall

clean:
	if [ -n "$(BUILDDIR)" -a -d "$(BUILDDIR)" ];     then rm -rf "$(BUILDDIR)"; fi
	if [ -n "$(INSTALLDIR)" -a -d "$(INSTALLDIR)" ]; then rm -rf "$(INSTALLDIR)"; fi

cleanall: clean
	rm -rf armv7a aarch64 i686 x86_64

.PHONY: build-libs/%
build-libs/%: $(INSTALLDIR)/%/lib/libogg.a $(INSTALLDIR)/%/lib/libvorbis.a $(INSTALLDIR)/%/lib/libFLAC.a \
	$(INSTALLDIR)/%/lib/libfreetype.a $(INSTALLDIR)/%/lib/libpng16.a $(INSTALLDIR)/%/lib/libharfbuzz.a \
	$(INSTALLDIR)/%/include/stb $(INSTALLDIR)/%/lib/libopenal.so $(INSTALLDIR)/%/lib/libsfml-main.a \
	$(INSTALLDIR)/%/lib/libglfw3.a $(INSTALLDIR)/%/lib/libnanovg.a
	@#

.PHONY: install-libs/%
install-libs/%: build-libs/%
	mkdir -p "$(INSTALLDIR)/$*/bin"
	mkdir -p "$(INSTALLDIR)/$*/lib"
	mkdir -p "$(INSTALLDIR)/$*/include"

	mkdir -p "$(DEPLOYDIR)/$*/include"
	( cd "$(INSTALLDIR)/$*/include" ; tar cf - . ) | ( cd "$(DEPLOYDIR)/$*/include" ; tar xf - )
	mkdir -p "$(DEPLOYDIR)/$*/bin"
	( cd "$(INSTALLDIR)/$*/bin" ; tar cf - . ) | ( cd "$(DEPLOYDIR)/$*/bin" ; tar xf - )
	mkdir -p "$(DEPLOYDIR)/$*/lib"
	( cd "$(INSTALLDIR)/$*/lib" ; tar cf - . ) | ( cd "$(DEPLOYDIR)/$*/lib" ; tar xf - )

	#~ find "$(DEPLOYDIR)/$*/bin" -type f -executable | while read -r FILE; do "$(NDK_BIN_$*)/strip" "$$FILE"; done
	#~ find "$(DEPLOYDIR)/$*/lib" -type f -name "*.a" | while read -r FILE; do "$(NDK_BIN_$*)/strip" "$$FILE"; done
	find "$(DEPLOYDIR)/$*/lib" -type f -name "*.a" | while read -r FILE; do "$(NDK_BIN_$*)/ranlib" "$$FILE"; done
	#~ find "$(DEPLOYDIR)/$*/lib" -type f -name "*.so*" | while read -r FILE; do "$(NDK_BIN_$*)/strip" "$$FILE"; done

.PHONY: install-libs-raw/%
install-libs-raw/%:
	mkdir -p "$(INSTALLDIR)/$*/bin"
	mkdir -p "$(INSTALLDIR)/$*/lib"
	mkdir -p "$(INSTALLDIR)/$*/include"

	mkdir -p "$(DEPLOYDIR)/$*/include"
	( cd "$(INSTALLDIR)/$*/include" ; tar cf - . ) | ( cd "$(DEPLOYDIR)/$*/include" ; tar xf - )
	mkdir -p "$(DEPLOYDIR)/$*/bin"
	( cd "$(INSTALLDIR)/$*/bin" ; tar cf - . ) | ( cd "$(DEPLOYDIR)/$*/bin" ; tar xf - )
	mkdir -p "$(DEPLOYDIR)/$*/lib"
	( cd "$(INSTALLDIR)/$*/lib" ; tar cf - . ) | ( cd "$(DEPLOYDIR)/$*/lib" ; tar xf - )

	#~ find "$(DEPLOYDIR)/$*/bin" -type f -executable | while read -r FILE; do "$(NDK_BIN_$*)/strip" "$$FILE"; done
	#~ find "$(DEPLOYDIR)/$*/lib" -type f -name "*.a" | while read -r FILE; do "$(NDK_BIN_$*)/strip" "$$FILE"; done
	find "$(DEPLOYDIR)/$*/lib" -type f -name "*.a" | while read -r FILE; do "$(NDK_BIN_$*)/ranlib" "$$FILE"; done
	#~ find "$(DEPLOYDIR)/$*/lib" -type f -name "*.so*" | while read -r FILE; do "$(NDK_BIN_$*)/strip" "$$FILE"; done


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
