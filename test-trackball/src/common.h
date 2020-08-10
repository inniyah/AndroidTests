#pragma once

#ifndef COMMON_H
#define COMMON_H

#if defined(__ANDROID__)

#include <android/log.h>
#define LOGV(...) (__android_log_print(ANDROID_LOG_VERBOSE, "GLFW3 Demo", __VA_ARGS__))
#define LOGI(...) (__android_log_print(ANDROID_LOG_INFO,    "GLFW3 Demo", __VA_ARGS__))
#define LOGW(...) (__android_log_print(ANDROID_LOG_WARN,    "GLFW3 Demo", __VA_ARGS__))
#define LOGE(...) (__android_log_print(ANDROID_LOG_ERROR,   "GLFW3 Demo", __VA_ARGS__))
#define LOGF(...) (__android_log_print(ANDROID_LOG_FATAL,   "GLFW3 Demo", __VA_ARGS__))

#include "AssetsManagement.h"

#define fopen asset_fopen

#else // ! __ANDROID__

#include <stdio.h>
#define LOGV(...) { printf("[DEBUG] "); printf(__VA_ARGS__); printf("\n"); }
#define LOGI(...) { printf("[INFO]  "); printf(__VA_ARGS__); printf("\n"); }
#define LOGW(...) { printf("[WARN]  "); printf(__VA_ARGS__); printf("\n"); }
#define LOGE(...) { printf("[ERROR] "); printf(__VA_ARGS__); printf("\n"); }
#define LOGF(...) { printf("[FATAL] "); printf(__VA_ARGS__); printf("\n"); }

#endif // ! __ANDROID__

#endif // COMMON_H
