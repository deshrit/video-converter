#pragma once

#include <iostream>
extern "C"
{
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
}

class Converter
{
private:
    std::string file_path;
    int video_stream_index, audio_stream_index;
    AVFormatContext *ifmt_context, *ofmt_context;
    AVCodecParameters *video_codec_params, *audio_codec_params;

private:
    void load_video_file();
    void set_video_stream_index();
    void set_audio_stream_index();

public:
    Converter(std::string file_path);
    ~Converter();

    void dump_video_info();
    void remux_to_mp4();
    void save_frame_as_jpeg(AVCodecContext *pCodecCtx, AVFrame *pFrame, unsigned int width);
    void extract_thumbnail();
};
