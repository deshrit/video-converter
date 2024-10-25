#pragma once

#include <iostream>
extern "C"
{
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavutil/avutil.h>
}

class Converter
{
private:
    std::string file_path;
    AVFormatContext *ifmt_context = nullptr, *ofmt_context = nullptr;

public:
    Converter(std::string &file_path);
    ~Converter();
    void convert_to_mp4();
};
