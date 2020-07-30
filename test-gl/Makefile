all: check test

APK_NAME = APKName
PACKAGE_NAME = com.android.gles3jni
ACTIVITY_NAME = GLES3JNIActivity

OBJS = gles3jni.o renderer.o renderer_gles2.o renderer_gles3.o
JAVA_SOURCES = $(shell find java -name "*.java")

include build.mk

ifeq ($(shell test $(ANDROID_API) -lt 18; echo $$?),0)
  OBJS += gl3stub.o
  JNI_DEFS += -DDYNAMIC_ES3
endif

clean:
	rm -rf build
