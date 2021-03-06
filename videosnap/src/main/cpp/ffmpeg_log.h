//
// Created by Chen on 2020/8/15.
//

#ifndef FILEBROWER_FFMPEG_LOG_H
#define FILEBROWER_FFMPEG_LOG_H
#define LOG_TAG "MediaMetadataRetrieverJNI"
#include <android/log.h>
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#endif //FILEBROWER_FFMPEG_LOG_H
