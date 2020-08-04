#ifndef DEMO_H
#define DEMO_H

#ifdef __cplusplus
extern "C" {
#endif

#if defined(__ANDROID__)
#include <android/log.h>
#define LOGV(...) (__android_log_print(ANDROID_LOG_VERBOSE, "NanoVG Demo", __VA_ARGS__))
#define LOGI(...) (__android_log_print(ANDROID_LOG_INFO,    "NanoVG Demo", __VA_ARGS__))
#define LOGW(...) (__android_log_print(ANDROID_LOG_WARN,    "NanoVG Demo", __VA_ARGS__))
#define LOGE(...) (__android_log_print(ANDROID_LOG_ERROR,   "NanoVG Demo", __VA_ARGS__))
#define LOGF(...) (__android_log_print(ANDROID_LOG_FATAL,   "NanoVG Demo", __VA_ARGS__))
#else
#include <stdio.h>
#define LOGV(...) { printf("[DEBUG] "); printf(__VA_ARGS__); printf("\n"); }
#define LOGI(...) { printf("[INFO]  "); printf(__VA_ARGS__); printf("\n"); }
#define LOGW(...) { printf("[WARN]  "); printf(__VA_ARGS__); printf("\n"); }
#define LOGE(...) { printf("[ERROR] "); printf(__VA_ARGS__); printf("\n"); }
#define LOGF(...) { printf("[FATAL] "); printf(__VA_ARGS__); printf("\n"); }
#endif

#include <nanovg/nanovg.h>

struct DemoData {
	int fontNormal, fontBold, fontIcons, fontEmoji;
	int images[12];
};
typedef struct DemoData DemoData;

int loadDemoData(NVGcontext* vg, DemoData* data);
void freeDemoData(NVGcontext* vg, DemoData* data);
void renderDemo(NVGcontext* vg, float mx, float my, float width, float height, float t, int blowup, DemoData* data);

#ifdef __cplusplus
}
#endif

#endif // DEMO_H
