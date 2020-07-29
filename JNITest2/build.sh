#!/bin/bash

# See: https://stackoverflow.com/questions/59504840/create-jni-ndk-apk-only-command-line-without-gradle-ant-or-cmake

set -euo pipefail

# Set the environment variables
################################

APK_NAME="APKName"
PACKAGE_NAME="some.some.some"
ORG_DIRS="${PACKAGE_NAME//./\/}"

SDK="/usr/lib/jvm/java-8-openjdk-amd64"
JAVAC="${SDK}/bin/javac"
JAVAH="${SDK}/bin/javah"
KEYTOOL="${SDK}/bin/keytool"

BUILD_TOOLS_DIR="/usr/lib/android-sdk/build-tools/debian"
AAPT="${BUILD_TOOLS_DIR}/aapt"
DX="${BUILD_TOOLS_DIR}/dx"
ZIPALIGN="${BUILD_TOOLS_DIR}/zipalign"
APKSIGNER="$(which -a apksigner || true)"
ADB="$(which -a adb || true)"

ANDROID_API=23
PLATFORM="/usr/lib/android-sdk/platforms/android-${ANDROID_API}/android.jar"

NDK="/usr/lib/android-ndk/"
CLANG_ARMV7A="${NDK}/toolchains/llvm/prebuilt/linux-x86_64/bin/armv7a-linux-androideabi23-clang"

apt_install=

if [ -z "${SDK}" -o ! -d "${SDK}" ]; then \
	echo "Package 'openjdk-8-jdk' is needed" && apt_install="${apt_install} openjdk-8-jdk"; fi
if [ -z "${JAVAC}" -o ! -x "${JAVAC}" ]; then \
	echo "Package 'openjdk-8-jdk' is needed" && apt_install="${apt_install} openjdk-8-jdk"; fi
if [ -z "${JAVAH}" -o ! -x "${JAVAH}" ]; then \
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

if [ -z "${NDK}" -o ! -d "${NDK}" ]; then \
	echo "Package 'google-android-ndk-installer' is needed" && apt_install="${apt_install} google-android-ndk-installer"; fi
if [ -z "${CLANG_ARMV7A}" -o ! -x "${CLANG_ARMV7A}" ]; then \
	echo "Package 'google-android-ndk-installer' is needed" && apt_install="${apt_install} google-android-ndk-installer"; fi

if [ -n "${apt_install}" ]; then \
	echo "WARN: sudo apt install${apt_install}" && false; fi

echo "Cleaning..."
rm -rf obj/*
rm -rf src/com/example/helloandroid/R.java


# Create project directories and files
#######################################

mkdir -p  src/"${ORG_DIRS}" res/layout build/gen build/obj build/apk jni

cat > ./AndroidManifest.xml << EOF
<?xml version="1.0" encoding="utf-8"?>
<manifest xmlns:android="http://schemas.android.com/apk/res/android"
          package="some.some.some"
          versionCode="1"
          versionName="0.1">
    <uses-sdk android:minSdkVersion="23"/>
    <application android:label="Hello">
        <activity android:name=".MainActivity">
            <intent-filter>
                <action android:name="android.intent.action.MAIN"/>
                <category android:name="android.intent.category.LAUNCHER"/>
            </intent-filter>
        </activity>
    </application>
</manifest>
EOF

cat > ./res/layout/main.xml << EOF
<?xml version="1.0" encoding="utf-8"?>
<LinearLayout
    xmlns:android="http://schemas.android.com/apk/res/android"
    android:layout_width="match_parent"
    android:layout_height="match_parent"
    android:gravity="center"
    android:orientation="vertical">

    <TextView
        android:layout_width="wrap_content"
        android:layout_height="wrap_content"
        android:id="@+id/my_text"/>
</LinearLayout>
EOF

cat > src/some/some/some/MainActivity.java << EOF
package some.some.some;

import android.app.Activity;
import android.os.Bundle;
import android.widget.TextView;

public class MainActivity extends Activity {
    static {
        System.loadLibrary("hello");
    }

    public native String getMessage();

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);

        TextView text = (TextView)findViewById(R.id.my_text);
        text.setText(getMessage());
    }
}
EOF


# Generate R.java file with Android Asset Packaging Tool (aapt)
################################################################

#~ The -f flag serves to aapt to overwrite any existing output file.
#~ The -m causes it to create package directories under the output directory.
#~ The -J makes it generate the R.java file and sets the output directory.
#~ The -S points out the resource directory. The -M specifies the manifest.
#~ The -I adds the platform .jar as an "include file".
#~ It all creates: build/gen/"${ORG_DIRS}"/R.java.

"${AAPT}" package -f -m -J build/gen/ -S res \
	-M AndroidManifest.xml -I "${PLATFORM}"


# Compile the java files with javac
####################################

"${JAVAC}" -bootclasspath "${SDK}/jre/lib/rt.jar" \
	-classpath "${PLATFORM}" -d build/obj \
	build/gen/"${ORG_DIRS}"/R.java src/"${ORG_DIRS}"/MainActivity.java


# Translate the .class files in build/obj/ to Dalvik byte code with dx tool
############################################################################

"${DX}" --dex --output=build/apk/classes.dex build/obj/


# Search the C function signature that corresponds to the Java method with javah tool
######################################################################################

mkdir -p tmp

"${JAVAH}" -classpath "${PLATFORM}:build/obj" \
      -o tmp/jni.h "${PACKAGE_NAME}".MainActivity

grep -A1 _getMessage tmp/jni.h


# Create hello.c file
######################

cat > jni/hello.c << EOF
#include <stdlib.h>
#include <jni.h>
#include <time.h>

static const char *const messages[] = {
  "Hello, world!",
  "Hej världen!",
  "Bonjour, monde!",
  "Hallo Welt!"
};

JNIEXPORT jstring JNICALL
Java_net_hanshq_hello_MainActivity_getMessage(JNIEnv *env, jobject obj) {
  int i;
  srand(time(NULL));
  i = rand() % (sizeof(messages) / sizeof(messages[0]));
  return (*env)->NewStringUTF(env, messages[i]);
}
EOF


# Create the libhello.so with arm toolchain
############################################

mkdir -p build/apk/lib/armeabi-v7a

"${CLANG_ARMV7A}" -shared -o build/apk/lib/armeabi-v7a/libhello.so jni/hello.c



# Package to create APK using the aapt tool again
##################################################

"${AAPT}" package -f -M AndroidManifest.xml -S res/ \
	-I "${PLATFORM}" \
	-F build/"${APK_NAME}".unsigned.apk build/apk/


# Using zipalign tool in the APK
#################################

"${ZIPALIGN}" -f -p 4 \
	build/"${APK_NAME}".unsigned.apk build/"${APK_NAME}".aligned.apk


# Create a key store and key for signing with the Java keytool
###############################################################

test -e keystore.jks || "${KEYTOOL}" -genkeypair -keystore keystore.jks -alias androidkey \
	-storepass android -keypass android \
	-validity 10000 -keyalg RSA -keysize 2048

"${KEYTOOL}" -list -v -keystore keystore.jks -storepass android -keypass android


# Sign the APK with apksigner tool
###################################

"${APKSIGNER}" sign --ks keystore.jks \
      --ks-key-alias androidkey --out build/"${APK_NAME}".apk \
      --ks-pass pass:android --key-pass pass:android \
      build/"${APK_NAME}".aligned.apk


# Check the APK content with aapt tool or jar tool
###################################################

# with aapt
"${AAPT}" list build/"${APK_NAME}".apk

# aapt with more details
"${AAPT}" list -v build/"${APK_NAME}".apk

# with jar
jar tf build/"${APK_NAME}".apk


# Test app with the adb tool
#############################

"${ADB}" install -r build/"${APK_NAME}".apk

# The --activity-clear-top is for clean the task avoiding the warning "Warning: Activity not started, its current task has been brought to the front".

"${ADB}" shell am start --activity-clear-top -n "${PACKAGE_NAME}"/.MainActivity
