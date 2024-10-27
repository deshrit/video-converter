// Utility functions for video converter

#pragma once

#include <iostream>
#include <tuple>
extern "C"
{
#include <libavcodec/avcodec.h>
}

const char *get_file_path_from_argv(const int argc, const char *argv[]);
int save_frame_as_jpeg(AVCodecContext *pCodecCtx, AVFrame *pFrame, int FrameNo);
std::tuple<std::string, std::string> get_filename_and_ext(std::string input_file_path);