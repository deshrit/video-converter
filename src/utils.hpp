// Utility functions for video converter

#pragma once

extern "C"
{
#include <libavcodec/avcodec.h>
}

const char *get_file_name_from_argv(const int argc, const char *argv[]);
int save_frame_as_jpeg(AVCodecContext *pCodecCtx, AVFrame *pFrame, int FrameNo);