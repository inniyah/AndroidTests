/*
 * The MIT License
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "AssetsManagement.h"

#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <istream>
#include <streambuf>
#include <vector>

#include <android/asset_manager.h>

static AAssetManager * android_asset_manager = NULL;

static int android_asset_read(void * cookie, char * buf, int size) {
    return AAsset_read((AAsset*)cookie, buf, size);
}

static int android_asset_write(void * cookie, const char * buf, int size) {
    (void)cookie; (void)buf; (void)size; // unused
    return EACCES; // can't provide write access to the apk
}

static fpos_t android_asset_seek(void * cookie, fpos_t offset, int whence) {
    return AAsset_seek((AAsset*)cookie, offset, whence);
}

static int android_asset_close(void * cookie) {
    AAsset_close((AAsset*)cookie);
    return 0;
}

void setAndroidAssetManager(AAssetManager * manager) {
    android_asset_manager = manager;
}

AAssetManager * getAndroidAssetManager() {
    return android_asset_manager;
}

// See: http://www.50ply.com/blog/2013/01/19/loading-compressed-android-assets-with-file-pointer/
extern "C" FILE * asset_fopen(const char * fname, const char * mode) {
  if (strchr(mode, 'w')) { // for writing
    LOGE("Unable to open asset (trying to write): '%s' ('%s')", fname, mode);
    return NULL;
  }
  if (!android_asset_manager) {
    LOGE("Unable to open asset (unknown asset manager): '%s' ('%s')", fname, mode);
    return NULL;
  }
  AAsset* asset = AAssetManager_open(android_asset_manager, fname, 0);
  if (!asset) {
    LOGW("Unable to open asset: '%s' ('%s')", fname, mode);
    return NULL;
  }
  LOGV("Opening asset: '%s' ('%s')", fname, mode);
  return funopen(asset, android_asset_read, android_asset_write, android_asset_seek, android_asset_close);
}
