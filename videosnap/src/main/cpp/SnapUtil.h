//
// Created by Chen on 2020/8/17.
//

#ifndef FILEBROWER_SNAPUTIL_H
#define FILEBROWER_MEDIAMETADATARETRIEVER_H

#include <unistd.h>

extern "C" {
#include "libavformat/avformat.h"
#include "libavutil/mem.h"
}

#include "android/bitmap.h"
#include "ffmpeg_log.h"
#include "yuv/libyuv.h"

class SnapUtil {
private:
    static bool debug;


public:
    SnapUtil();

    bool getScaledFrame(int fd, int width, int height, uint8_t *data);

    ~SnapUtil() {
    }
};


#endif //FILEBROWER_SNAPUTIL_H
