#pragma once

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

#include "common.h"

#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <istream>
#include <streambuf>
#include <vector>

#ifndef ASSETS_MANAGEMENT_H
#define ASSETS_MANAGEMENT_H

#if defined(__ANDROID__)

#include <android/asset_manager.h>

void setAndroidAssetManager(AAssetManager * manager);
AAssetManager * getAndroidAssetManager();

extern "C" FILE * asset_fopen(const char * fname, const char * mode);

// See: https://stackoverflow.com/questions/11063786/can-i-use-ifstream-in-android-ndk-to-access-assets
class asset_streambuf : public std::streambuf {
public:
    asset_streambuf(const std::string& filename) : asset(nullptr) {
        if (!getAndroidAssetManager()) {
            LOGE("Unable to open asset (unknown asset manager): '%s'", filename.c_str());
            return;
         }

        asset = AAssetManager_open(getAndroidAssetManager(), filename.c_str(), AASSET_MODE_STREAMING);

        if (!asset) {
            LOGW("Unable to open asset: '%s'", filename.c_str());
            return;
        } else {
            LOGV("Opening asset: '%s'", filename.c_str());
        }

        buffer.resize(1024);

        setg(0, 0, 0);
        setp(&buffer.front(), &buffer.front() + buffer.size());
    }

    virtual ~asset_streambuf() {
        LOGV("Closing asset");
        sync();
        if (asset) AAsset_close(asset);
        asset = nullptr;
    }

    std::streambuf::int_type underflow() override {
        auto bufferPtr = &buffer.front();
        auto counter = AAsset_read(asset, bufferPtr, buffer.size());

        if(counter == 0)
            return traits_type::eof();

        if(counter < 0) // error, what to do now?
            return traits_type::eof();

        setg(bufferPtr, bufferPtr, bufferPtr + counter);

        return traits_type::to_int_type(*gptr());
    }

    std::streambuf::int_type overflow(std::streambuf::int_type value) override {
        return traits_type::eof();
    }

    int sync() override {
        std::streambuf::int_type result = overflow(traits_type::eof());
        return traits_type::eq_int_type(result, traits_type::eof()) ? -1 : 0;
    }

    bool is_open() const {
        return (asset != nullptr);
    }

    void close() {
        sync();
        if (asset) AAsset_close(asset);
        asset = nullptr;
    }

private:
    AAsset* asset;
    std::vector<char> buffer;
};

class asset_istream: public std::istream {
public:
    asset_istream(const std::string& file) : std::istream(new asset_streambuf(file)) {
    }

    virtual ~asset_istream() {
        close();
    }

    //~ void open(const std::string & path, std::ios::openmode mode) {}

    bool is_open() const {
        return (rdbuf() != nullptr);
    }

    void close() {
        if (rdbuf() != nullptr) {
            delete rdbuf();
            rdbuf(nullptr);
        }
    }
};

#endif // __ANDROID__

#endif // ASSETS_MANAGEMENT_H
