package com.ffmpeg;

import android.graphics.Bitmap;
import android.util.Log;


public class VideoSnap {
    private static final String[] librarys = {
            "ffmpeg_core",
            "ffmpeg"
    };

    static {
        for (String lib : librarys) {
            try {
                System.loadLibrary(lib);
            } catch (UnsatisfiedLinkError e) {
                e.printStackTrace();
            }
        }

    }

    public boolean getScaledSnap(int fd, Bitmap bitmap, int width, int height) {
        if (bitmap == null || bitmap.isRecycled()) {
            return false;
        }
        if (bitmap.getWidth() != width || bitmap.getHeight() != height) {
            return false;
        }
        return nativeGetScaledSnap(fd, bitmap, width, height);
    }

    private static native boolean nativeGetScaledSnap(int fd, Bitmap bitmap, int width, int height);

}
