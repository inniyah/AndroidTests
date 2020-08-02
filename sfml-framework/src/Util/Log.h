#pragma once

#ifdef ANDROID
#include <android/log.h>
#define LOGV(...) (__android_log_print(ANDROID_LOG_VERBOSE, "GAME", __VA_ARGS__))
#define LOGI(...) (__android_log_print(ANDROID_LOG_INFO, "GAME", __VA_ARGS__))
#define LOGW(...) (__android_log_print(ANDROID_LOG_WARN, "GAME", __VA_ARGS__))
#define LOGE(...) (__android_log_print(ANDROID_LOG_ERROR, "GAME", __VA_ARGS__))
#define LOGF(...) (__android_log_print(ANDROID_LOG_FATAL, "GAME", __VA_ARGS__))
#else
#include <stdio.h>
#define LOGV(...) { printf("[DEBUG] "); printf(__VA_ARGS__); printf("\n"); }
#define LOGI(...) { printf("[INFO]  "); printf(__VA_ARGS__); printf("\n"); }
#define LOGW(...) { printf("[WARN]  "); printf(__VA_ARGS__); printf("\n"); }
#define LOGE(...) { printf("[ERROR] "); printf(__VA_ARGS__); printf("\n"); }
#define LOGF(...) { printf("[FATAL] "); printf(__VA_ARGS__); printf("\n"); }
#endif
