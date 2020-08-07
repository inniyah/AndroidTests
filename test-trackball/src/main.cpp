#define GLFW_INCLUDE_ES3
#include <GLFW/glfw3.h>

#include "TrackballControls.h"

#if defined(__ANDROID__)
#include <android/log.h>
#define LOGV(...) (__android_log_print(ANDROID_LOG_VERBOSE, "GLFW Demo", __VA_ARGS__))
#define LOGI(...) (__android_log_print(ANDROID_LOG_INFO,    "GLFW Demo", __VA_ARGS__))
#define LOGW(...) (__android_log_print(ANDROID_LOG_WARN,    "GLFW Demo", __VA_ARGS__))
#define LOGE(...) (__android_log_print(ANDROID_LOG_ERROR,   "GLFW Demo", __VA_ARGS__))
#define LOGF(...) (__android_log_print(ANDROID_LOG_FATAL,   "GLFW Demo", __VA_ARGS__))
#else
#include <stdio.h>
#define LOGV(...) { printf("[DEBUG] "); printf(__VA_ARGS__); printf("\n"); }
#define LOGI(...) { printf("[INFO]  "); printf(__VA_ARGS__); printf("\n"); }
#define LOGW(...) { printf("[WARN]  "); printf(__VA_ARGS__); printf("\n"); }
#define LOGE(...) { printf("[ERROR] "); printf(__VA_ARGS__); printf("\n"); }
#define LOGF(...) { printf("[FATAL] "); printf(__VA_ARGS__); printf("\n"); }
#endif

int main() {
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
    glfwWindowHint(GLFW_CONTEXT_CREATION_API, GLFW_EGL_CONTEXT_API);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    GLFWwindow* window = glfwCreateWindow(1920, 1080, "Window Title", NULL, NULL);
    glfwMakeContextCurrent(window);

    while (1) {
        glClearColor(0.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glfwSwapBuffers(window);
    }
}
