#!/bin/bash

# See: https://medium.com/@authmane512/how-to-build-an-apk-from-command-line-without-ide-7260e1e22676

set -euo pipefail

OPENJDK_DIR="/usr/lib/jvm/java-8-openjdk-amd64"
JAVAC="${OPENJDK_DIR}/bin/javac"
KEYTOOL="${OPENJDK_DIR}/bin/keytool"

BUILD_TOOLS_DIR="/usr/lib/android-sdk/build-tools/debian"
AAPT="${BUILD_TOOLS_DIR}/aapt"
DX="${BUILD_TOOLS_DIR}/dx"
ZIPALIGN="${BUILD_TOOLS_DIR}/zipalign"
APKSIGNER="$(which -a apksigner || true)"
ADB="$(which -a adb || true)"
PLATFORM="$(ls /usr/lib/android-sdk/platforms/*/android.jar || true | sort | head -n 1)"

apt_install=
if [ -z "${OPENJDK_DIR}" -o ! -d "${OPENJDK_DIR}" ]; then \
	echo "Package 'openjdk-8-jdk' is needed" && apt_install="${apt_install} openjdk-8-jdk"; fi
if [ -z "${JAVAC}" -o ! -x "${JAVAC}" ]; then \
	echo "Package 'openjdk-8-jdk' is needed" && apt_install="${apt_install} openjdk-8-jdk"; fi
if [ -z "${KEYTOOL}" -o ! -x "${KEYTOOL}" ]; then \
	echo "Package 'openjdk-8-jre' is needed" && apt_install="${apt_install} openjdk-8-jre"; fi
if [ -z "${BUILD_TOOLS_DIR}" -o ! -d "${BUILD_TOOLS_DIR}" ]; then \
	echo "Package 'android-sdk' is needed" && apt_install="${apt_install} android-sdk"; fi
if [ -z "${AAPT}" -o ! -x "${AAPT}" ]; then \
	echo "Package 'aapt' is needed" && apt_install="${apt_install} aapt"; fi
if [ -z "${DX}" -o ! -x "${DX}" ]; then \
	echo "Package 'dalvik-exchange' is needed" && apt_install="${apt_install} dalvik-exchange"; fi
if [ -z "${ZIPALIGN}" -o ! -x "${ZIPALIGN}" ]; then \
	echo "Package 'zipalign' is needed" && apt_install="${apt_install} zipalign"; fi
if [ -z "${APKSIGNER}" -o ! -x "${APKSIGNER}" ]; then \
	echo "Package 'apksigner' is needed" && apt_install="${apt_install} apksigner"; fi
if [ -z "${ADB}" -o ! -x "${ADB}" ]; then \
	echo "Package 'adb' is needed" && apt_install="${apt_install} adb"; fi
if [ -z "${PLATFORM}" -o ! -r "${PLATFORM}" ]; then \
	echo "Package 'libandroid-23-java' is needed" && apt_install="${apt_install} libandroid-23-java"; fi
if [ -n "${apt_install}" ]; then \
	echo "WARN: sudo apt install${apt_install}" && false; fi

echo "Cleaning..."
rm -rf obj/*
rm -rf src/com/example/helloandroid/R.java

mkdir -p bin
mkdir -p obj
mkdir -p src/com/example/helloandroid

echo "Generating R.java file..."
"${AAPT}" package -f -m -J src -M AndroidManifest.xml -S res -I "$PLATFORM"

echo "Compiling..."
"${JAVAC}" -d obj -classpath src -bootclasspath "${PLATFORM}" -source 1.7 -target 1.7 \
	src/com/example/helloandroid/MainActivity.java
"${JAVAC}" -d obj -classpath src -bootclasspath "${PLATFORM}" -source 1.7 -target 1.7 \
	src/com/example/helloandroid/R.java

echo "Translating in Dalvik bytecode..."
"${DX}" --dex --output=classes.dex $(ls libs/*.jar) obj

echo "Making APK..."
"${AAPT}" package -f -m -F bin/hello.unaligned.apk -M AndroidManifest.xml -S res -I "${PLATFORM}"
"${AAPT}" add bin/hello.unaligned.apk classes.dex

echo "Make sure that the keys are there..."
test -e debug.keystore || "${KEYTOOL}" -genkey -v -keystore debug.keystore -storepass android \
	-alias androiddebugkey -keypass android -keyalg RSA -keysize 2048 -validity 10000
"${KEYTOOL}" -list -v -keystore debug.keystore -storepass android -keypass android

echo "Aligning and signing APK..."
"${APKSIGNER}" sign --ks ./debug.keystore --ks-key-alias androiddebugkey --ks-pass pass:android \
	--key-pass pass:android bin/hello.unaligned.apk
"${ZIPALIGN}" -f 4 bin/hello.unaligned.apk bin/hello.apk

if [ $# -ge 1 ] && [ "${1:-}" == "test" ]; then
	echo "Launching..."
	"${ADB}" install -r bin/hello.apk
	"${ADB}" shell am start -n com.example.helloandroid/.MainActivity
fi
