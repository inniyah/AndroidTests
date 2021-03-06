//
// Copyright (c) 2013 Mikko Mononen memon@inside.org
//
// This software is provided 'as-is', without any express or implied
// warranty.  In no event will the authors be held liable for any damages
// arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not
//    claim that you wrote the original software. If you use this software
//    in a product, an acknowledgment in the product documentation would be
//    appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be
//    misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.
//

#define GLFW_INCLUDE_ES3
#include <GLFW/glfw3.h>

#if defined(__ANDROID__)
#define GLFW_EXPOSE_NATIVE_ANDROID
#include <GLFW/glfw3native.h>
#endif

#include "demo.h"

#include <nanovg/nanovg.h>
#include <nanovg/nanovg_gl.h>

static void error_cb(int error, const char* desc) {
	LOGE("GLFW error %d: %s\n", error, desc);
}

int blowup = 0;
int premult = 0;

static void key(GLFWwindow * window, int key, int scancode, int action, int mods)
{
	NVG_NOTUSED(scancode);
	NVG_NOTUSED(mods);
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
		blowup = !blowup;
	if (key == GLFW_KEY_P && action == GLFW_PRESS)
		premult = !premult;
}

int main() {
	DemoData data;

	if (!glfwInit()) {
		LOGF("Failed to init GLFW.");
		return -1;
	}

	glfwSetErrorCallback(error_cb);

#ifndef _WIN32 // don't require this on win32, and works with more cards
	glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
	glfwWindowHint(GLFW_CONTEXT_CREATION_API, GLFW_EGL_CONTEXT_API);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
#endif

	#ifdef DEMO_MSAA
		glfwWindowHint(GLFW_SAMPLES, 4);
	#endif

	GLFWwindow * window = glfwCreateWindow(1000, 600, "NanoVG", NULL, NULL);

	if (!window) {
		glfwTerminate();
		return -1;
	}

#if defined(__ANDROID__)
	struct android_app * cyborg_app = glfwGetAndroidApp(window);
	JNIEnv * cyborg_env = cyborg_app->activity->env;
	AAssetManager * cybord_ass = cyborg_app->activity->assetManager;
	nvgSetAndroidAssetManager(cybord_ass);
#endif

	glfwSetKeyCallback(window, key);

	glfwMakeContextCurrent(window);

	#ifdef DEMO_MSAA
		NVGcontext * vg = nvgCreateGLES3(NVG_STENCIL_STROKES | NVG_DEBUG);
	#else
		NVGcontext * vg = nvgCreateGLES3(NVG_ANTIALIAS | NVG_STENCIL_STROKES | NVG_DEBUG);
	#endif

	if (vg == NULL) {
		LOGF("Could not init nanovg.\n");
		return -1;
	}

	if (loadDemoData(vg, &data) == -1) {
		LOGE("Could not load demo data.\n");
	}

	glfwSwapInterval(0);

	glfwSetTime(0);
	//prevt = glfwGetTime();

	while (!glfwWindowShouldClose(window)) {
		double mx, my, t;
		int winWidth, winHeight;
		int fbWidth, fbHeight;
		float pxRatio;

		t = glfwGetTime();
		//double dt = t - prevt;
		//prevt = t;

		glfwGetCursorPos(window, &mx, &my);
		glfwGetWindowSize(window, &winWidth, &winHeight);
		glfwGetFramebufferSize(window, &fbWidth, &fbHeight);

		// Calculate pixel ration for hi-dpi devices.
		pxRatio = (float)fbWidth / (float)winWidth;

		// Update and render
		glViewport(0, 0, fbWidth, fbHeight);

		if (premult) {
			glClearColor(0,0,0,0);
		} else {
			glClearColor(0.3f, 0.3f, 0.32f, 1.0f);
		}

		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);

		nvgBeginFrame(vg, winWidth, winHeight, pxRatio);

		renderDemo(vg, mx,my, winWidth,winHeight, t, blowup, &data);

		nvgEndFrame(vg);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	freeDemoData(vg, &data);

	nvgDeleteGLES3(vg);

	glfwTerminate();
	return 0;
}
