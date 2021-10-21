//
// Created by Chen on 2020/8/17.
//

#include <assert.h>
#include "jni_common.h"


static JNIEXPORT jboolean JNICALL
nativeGetScaledSnap(JNIEnv *env, jclass clazz, jint fd, jobject bitmap, jint width,
                    jint height) {
    AndroidBitmapInfo *androidBitmapInfo = nullptr;
    if (AndroidBitmap_getInfo(env, bitmap, androidBitmapInfo) < 0) {
        return JNI_FALSE;
    }
    void *data = nullptr;
    if (AndroidBitmap_lockPixels(env, bitmap, &data) < 0) {
        return JNI_FALSE;
    }
    auto *snap = new SnapUtil();
    bool result = snap->getScaledFrame(fd, width, height,
                                       static_cast<uint8_t *>(data));
    delete snap;
    AndroidBitmap_unlockPixels(env, bitmap);
    if (result) {
        return JNI_TRUE;
    } else {
        return JNI_FALSE;
    }
}


static JNINativeMethod methods[] = {
        {"nativeGetScaledSnap", "(ILandroid/graphics/Bitmap;II)Z", (void *) nativeGetScaledSnap},
};


static int common_load(JNIEnv *env) {
    const char *className = "com/ffmpeg/VideoSnap";
    jclass clazz;
    clazz = env->FindClass(className);
    if (clazz == nullptr) {
        return -1;
    }
    return env->RegisterNatives(clazz, methods, METHOD_COUNT(methods));
}

jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved) {
    JNIEnv *env = nullptr;
    if (vm->GetEnv(reinterpret_cast<void **>(&env), JNI_VERSION_1_6) != JNI_OK) {
        return -1;
    }
    assert(env != nullptr);
    if (common_load(env)) {
        return -1;
    }
    return JNI_VERSION_1_6;
}



