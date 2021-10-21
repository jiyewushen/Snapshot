//
// Created by Chen on 2020/8/17.
//


#include <sys/stat.h>
#include <exception>
#include "SnapUtil.h"

bool SnapUtil::debug = false;

static int read_packet(void *opaque, uint8_t *buf, int buf_size) {
    int fd = *(static_cast<int *>(opaque));
    int ret = read(fd, buf, buf_size);
    if (ret == 0) {
        return AVERROR_EOF;
    }
    return (ret == -1) ? AVERROR(errno) : ret;
}

static int write_packet(void *opaque, uint8_t *buf, int buf_size) {
    int fd = *(static_cast<int *>(opaque));
    int ret = write(fd, buf, buf_size);
    return (ret == -1) ? AVERROR(errno) : ret;
}

static int64_t seek(void *opaque, int64_t offset, int whence) {
    int fd = *(static_cast<int *>(opaque));
    int64_t ret;
    if (whence == AVSEEK_SIZE) {
        struct stat64 st;
        ret = fstat64(fd, &st);
        return ret < 0 ? AVERROR(errno) : (S_ISFIFO(st.st_mode) ? 0 : st.st_size);
    }
    ret = lseek64(fd, offset, whence);
    return ret < 0 ? AVERROR(errno) : ret;
}

// 对FFmpeg日志进行格式化
static void ffp_log_callback_report(void *ptr, int level, const char *fmt, va_list vl) {
    va_list vl2;
    char line[2048];
    static int print_prefix = 1;
    va_copy(vl2, vl);
    av_log_format_line(ptr, level, fmt, vl2, line, sizeof(line), &print_prefix);
    va_end(vl2);
    if (level <= AV_LOG_ERROR) {
        LOGE("%s", line);
    }else{
        LOGD("%s",line);
    }
}

SnapUtil::SnapUtil() {
    if (debug) {
        av_log_set_level(AV_LOG_DEBUG);
        av_log_set_callback(ffp_log_callback_report);
    }
}

void drawBitmap(AVFrame *frame, int outWidth, int outHeight, uint8_t *data) {
    if (frame->format != AV_PIX_FMT_YUV420P) {
        LOGE("format is not 420 %d", frame->format);
//        av_frame_free(&frame);
        return;
    }
    if (frame->data[0] == nullptr || frame->data[1] == nullptr || frame->data[2] == nullptr) {
        LOGE("format data is null");
        return;
    }
    int srcW = frame->width;
    int srcH = frame->height;
    if (srcW < 1 || srcH < 1) {
        LOGE("format width %d or height %d not right", srcW, srcH);
        return;
    }
    float wScale = srcW * 1.0F / outWidth;
    float hScale = srcH * 1.0F / outHeight;
    int cropWidth;
    int cropHeight;
    int cropX;
    int cropY;
    bool isCropScale = false;
    bool isScaleCrop = false;
    //如果长宽的缩放比例不等时，需要确定裁剪区域
    if (wScale != hScale) {
        float s = fmin(wScale, hScale);
        if (s >= 2.0F) {
            srcW = frame->width / s;
            srcH = frame->height / s;
//            LOGE("是否需要先缩后裁 %f 原始 %d %d 缩放后: %d %d 输出：%d %d", s, frame->width, frame->height, srcW,
//                 srcH, outWidth, outHeight);
            if (srcH < outHeight) {
                srcH = outHeight;
            }
            if (srcW < outWidth) {
                srcW = outWidth;
            }
            isScaleCrop = true;
            cropWidth = outWidth;
            cropHeight = outHeight;
        } else {
            isCropScale = true;
            if (srcW > srcH) {
                cropWidth = srcH;
                cropHeight = srcH;
            } else {
                cropWidth = srcW;
                cropHeight = srcW;
            }
        }
        cropX = (srcW - cropWidth) / 2;
        cropY = (srcH - cropHeight) / 2;
        if ((cropX & 0b01) != 0) {
            cropX -= 1;
        }
        if ((cropY & 0b01) != 0) {
            cropY -= 1;
        }
//        LOGE("srcW %d,srcH %d; cropW %d,cropH %d; cropX %d,cropY %d", srcW, srcH, cropWidth,
//             cropHeight, cropX, cropY);
    }

    int halfWidth = outWidth >> 1;
    int outWH = outWidth * outHeight;
    auto *temp = new uint8_t[outWH * 3 / 2];
    uint8_t *temp_u = temp + outWH;
    uint8_t *temp_v = temp + outWH * 5 / 4;

    if (isCropScale) {
        int hw = cropWidth >> 1;
        int wh = cropWidth * cropHeight;
        auto *crop = new uint8_t[wh * 3 / 2];
        uint8_t *crop_u = crop + wh;
        uint8_t *crop_v = crop + wh * 5 / 4;

//        int halfwidth = srcW >> 1;
        uint8_t *src_y = frame->data[0] + (frame->linesize[0] * cropY + cropX);
        uint8_t *src_u = frame->data[1] + frame->linesize[1] * (cropY / 2) + (cropX / 2);
        uint8_t *src_v = frame->data[2] + frame->linesize[2] * (cropY / 2) + (cropX / 2);
//        某些图片原始宽度不等于frame->linesize[0]导致花屏，fuck!
//        LOGE("%d %d %d %d %d", srcW, halfwidth, frame->linesize[0], frame->linesize[1],
//             frame->linesize[2]);
        int result = libyuv::I420Rotate(src_y, frame->linesize[0], src_u, frame->linesize[1], src_v,
                                        frame->linesize[2],
                                        crop, cropWidth,
                                        crop_u, hw,
                                        crop_v, hw,
                                        cropWidth, cropHeight, libyuv::kRotate0);
        result = libyuv::I420Scale(
                crop, cropWidth,
                crop_u, hw,
                crop_v, hw,
                cropWidth, cropHeight,
                temp, outWidth,
                temp_u, halfWidth,
                temp_v, halfWidth,
                outWidth, outHeight,
                libyuv::FilterModeEnum::kFilterNone
        );
        delete[]crop;
    } else if (isScaleCrop) {
        int hw = srcW >> 1;
        int wh = srcW * srcH;
        auto *scale = new uint8_t[wh * 3 / 2];
        uint8_t *scale_u = scale + wh;
        uint8_t *scale_v = scale + wh * 5 / 4;
        int result = libyuv::I420Scale(
                frame->data[0], frame->linesize[0],
                frame->data[1], frame->linesize[1],
                frame->data[2], frame->linesize[2],
                frame->width, frame->height,
                scale, srcW,
                scale_u, hw,
                scale_v, hw,
                srcW, srcH,
                libyuv::FilterModeEnum::kFilterNone
        );
//        result=libyuv::ConvertToI420(scale, srcW * 3 / 2, temp, outWidth, temp_u, halfWidth, temp_v,
//                              halfWidth, cropX, cropY,srcW,srcH,cropWidth,cropHeight,libyuv::kRotate0,libyuv::FOURCC_I420);
        uint8_t *src_y = scale + (srcW * cropY + cropX);
        uint8_t *src_u = scale_u + hw * (cropY / 2) + (cropX / 2);
        uint8_t *src_v = scale_v + hw * (cropY / 2) + (cropX / 2);
        result = libyuv::I420Rotate(src_y, srcW, src_u, hw, src_v, hw,
                                    temp, outWidth,
                                    temp_u, halfWidth,
                                    temp_v, halfWidth,
                                    cropWidth, cropHeight, libyuv::kRotate0);
        delete[]scale;
    } else {
        libyuv::I420Scale(
                frame->data[0], frame->linesize[0],
                frame->data[1], frame->linesize[1],
                frame->data[2], frame->linesize[2],
                frame->width, frame->height,
                temp, outWidth,
                temp_u, halfWidth,
                temp_v, halfWidth,
                outWidth, outHeight,
                libyuv::FilterModeEnum::kFilterNone
        );
    }
    int linesize = outWidth * 4;
    libyuv::I420ToABGR(
            temp, outWidth,//Y
            temp_u, halfWidth,//U
            temp_v, halfWidth,// V
            data, linesize,  // RGBA
            outWidth, outHeight);
    delete[]temp;
}

static int flushDecode(AVCodecContext *dec, const AVPacket *pkt, AVFrame *frame) {
    int ret = 0;

    // submit the packet to the decoder
    ret = avcodec_send_packet(dec, pkt);
    if (ret < 0) {
        return ret;
    }
    // get all the available frames from the decoder
    while (ret >= 0) {
        ret = avcodec_receive_frame(dec, frame);
        if (ret < 0) {
            // those two return values are special and mean there is no output
            // frame available, but there were no errors during decoding
            if (ret == AVERROR_EOF || ret == AVERROR(EAGAIN))
                return 0;

            return ret;
        }

        av_frame_unref(frame);
        if (ret < 0)
            return ret;
    }

    return 0;
}

static void
close(AVCodecContext *avCodecContext, AVFormatContext *avFormatContext,
      AVIOContext *avioContext,
      AVPacket *packet, AVFrame *frame) {
    if (frame != nullptr) {
        av_frame_free(&frame);
    }
    if (packet != nullptr) {
        av_packet_free(&packet);
    }
    if (avCodecContext != nullptr) {
        avcodec_free_context(&avCodecContext);
    }
    if (avFormatContext != nullptr) {
        avformat_close_input(&avFormatContext);
    }
    if (avioContext != nullptr) {
//        if (avioContext->buffer != nullptr) {
//            av_freep(&avioContext->buffer);
//        }
        avio_context_free(&avioContext);
    }
}

bool SnapUtil::getScaledFrame(int fd, int width, int height,
                              uint8_t *data) {
    if (fd < 0) {
        return false;
    }
    struct stat st;
    if (fstat(fd, &st) < 0) {
        return false;
    }
    if (data == nullptr) {
        return false;
    }
    size_t bufferSize = 4 * 1024;

    if (st.st_size < bufferSize) {
        bufferSize = st.st_size;
    }
    if (bufferSize <= 0) {
        return false;
    }

    auto *buffer = static_cast<uint8_t *>(av_malloc(bufferSize));
    if (buffer == nullptr) {
        return false;
    }
    AVIOContext *avioContext = avio_alloc_context(buffer, bufferSize, 0, &fd, &read_packet,
                                                  &write_packet,
                                                  &seek);
    if (avioContext == nullptr) {
        av_freep(buffer);
        return false;
    }
    AVFormatContext *avFormatContext = avformat_alloc_context();
    if (avFormatContext == nullptr) {
        close(nullptr, nullptr, avioContext, nullptr, nullptr);
        return false;
    }
    avFormatContext->pb = avioContext;
    avFormatContext->flags |= AVFMT_FLAG_CUSTOM_IO;
    avFormatContext->fps_probe_size = 2;
    int ret = 0;
    if ((ret = avformat_open_input(&avFormatContext, nullptr, nullptr, nullptr)) < 0) {
        if (debug) {
            LOGE("avformat_open_input %s", av_err2str(ret));
        }
        close(nullptr, avFormatContext, avioContext, nullptr, nullptr);
        //解封文件失败
        return false;
    }

    if ((ret = avformat_find_stream_info(avFormatContext, nullptr)) < 0) {
        if (debug) {
            LOGE("avformat_find_stream_info %s", av_err2str(ret));
        }
        close(nullptr, avFormatContext, avioContext, nullptr, nullptr);
        return false;
    }
    const AVCodec *dec = nullptr;
    int videoStreamIndex = av_find_best_stream(avFormatContext, AVMEDIA_TYPE_VIDEO, -1, -1,
                                               const_cast<AVCodec **>(&dec),
                                               0);

    if (videoStreamIndex < 0) {
        if (debug) {
            LOGE("av_find_best_stream");
        }
        //无法找视频流
        close(nullptr, avFormatContext, avioContext, nullptr, nullptr);
        return false;
    }

    AVStream *stream = avFormatContext->streams[videoStreamIndex];
    if (stream->codecpar == nullptr) {
        close(nullptr, avFormatContext, avioContext, nullptr, nullptr);
        return false;
    }

//    AVCodec *avCodec = avcodec_find_decoder(stream->codecpar->codec_id);
//    if (!avCodec) {
//        if (debug)
//            LOGE("Failed to find %s codec\n",
//                 av_get_media_type_string(AVMEDIA_TYPE_VIDEO));
//        close(nullptr, avFormatContext, avioContext, nullptr, nullptr);
//        return false;
//    }

    AVCodecContext *avCodecContext = avcodec_alloc_context3(dec);
    if (!avCodecContext) {
        //Failed to allocate codec
        close(avCodecContext, avFormatContext, avioContext, nullptr, nullptr);
        return false;
    }

    //将codecpar中的信息复制到AVCodecContext中
    if (avcodec_parameters_to_context(avCodecContext, stream->codecpar) < 0) {
        //复制失败
        close(avCodecContext, avFormatContext, avioContext, nullptr, nullptr);
        return false;
    }

    if (avcodec_open2(avCodecContext, dec, nullptr) < 0) {
        close(avCodecContext, avFormatContext, avioContext, nullptr, nullptr);
        return false;
    }

    AVFrame *frame = av_frame_alloc();
    if (!frame) {
        if (debug) {
            LOGE("av_frame can't alloc");
        }
        close(avCodecContext, avFormatContext, avioContext, nullptr, frame);
        return false;
    }
    AVPacket *packet = av_packet_alloc();
    if (!packet) {
        if (debug) {
            LOGE("av_packet can't alloc");
        }
        close(avCodecContext, avFormatContext, avioContext, packet, frame);
        return false;
    }

    ret = 0;
    int result = 0;
    bool decodeResult = false;
    while ((result = av_read_frame(avFormatContext, packet)) >= 0) {
        if (packet->stream_index != videoStreamIndex) {
//            av_packet_unref(packet);
            continue;
        }
        // submit the packet to the decoder
        ret = avcodec_send_packet(avCodecContext, packet);
        if (ret < 0 && ret != AVERROR(EAGAIN)) {
            av_packet_unref(packet);
            if (debug) {
                LOGE("avcodec_send_packet < 0 %s %d", av_err2str(ret), ret);
            }
            break;
        }
        //将avcodec_receive_frame状态置为0，保证avcodec_send_packet状态在AVERROR(EAGAIN)时，avcodec_receive_frame能够正确执行。
        ret = 0;
        // get all the available frames from the decoder
        while (ret >= 0) {
            ret = avcodec_receive_frame(avCodecContext, frame);
            if (ret >= 0) {
                drawBitmap(frame, width, height, data);
                av_frame_unref(frame);
                decodeResult = true;
                break;
            }
            av_frame_unref(frame);
        }

        av_packet_unref(packet);
        if (ret < 0) {
//            // those two return values are special and mean there is no output
//            // frame available, but there were no errors during decoding
////            if (ret == AVERROR_EOF || ret == AVERROR(EAGAIN)) {
//            if (ret == AVERROR_EOF) {
//                avcodec_flush_buffers(avCodecContext);
//                return ret;
//            }
            if (ret == AVERROR(EAGAIN)) {
                if (debug) {
                    LOGE("avcodec_receive_frame  %s %d", av_err2str(ret), ret);
                }
                continue;
            }
            if (ret != AVERROR_EOF) {
//                av_packet_unref(packet);
                break;
            }
        }


        if (ret >= 0 || ret == AVERROR_EOF) {
//            flushDecode(avCodecContext, nullptr, nullptr);
            result = 0;
            break;
        }
    }
    close(avCodecContext, avFormatContext, avioContext, packet, frame);
    return decodeResult;
}



