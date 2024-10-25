#include "converter.hpp"

Converter::Converter(std::string &file_path)
{
    // Load video file
    int ret = avformat_open_input(&ifmt_context, file_path.c_str(), nullptr, nullptr);
    if (ret != 0)
    {
        std::cerr << "Unable to open file '" << file_path << "' error code " << ret << std::endl;
        exit(EXIT_FAILURE);
    }
    ret = avformat_find_stream_info(ifmt_context, nullptr);
    if (ret != 0)
    {
        std::cerr << "Unable to find stream info of file '" << file_path << "' error code " << ret << std::endl;
        exit(EXIT_FAILURE);
    }
    av_dump_format(ifmt_context, 0, file_path.c_str(), 0);

    // Check if video and audio stream exists
    int video_stream_index = -1;
    int audio_stream_index = -1;
    AVCodecParameters *codec_params;
    for (int i = 0; i < ifmt_context->nb_streams; i++)
    {
        auto stream = ifmt_context->streams[i];
        codec_params = stream->codecpar;
        if (codec_params->codec_type == AVMEDIA_TYPE_VIDEO)
            video_stream_index = 1;
        if (codec_params->codec_type == AVMEDIA_TYPE_AUDIO)
            audio_stream_index = 1;
    }
    if (video_stream_index == -1)
    {
        std::cout << "Unable to find video stream in file '" << file_path << "'\n";
        exit(EXIT_FAILURE);
    }
    if (audio_stream_index == -1)
    {
        std::cout << "Unable to find audio stream in file '" << file_path << "'\n";
    }

    av_dump_format(ifmt_context, video_stream_index, file_path.c_str(), 0);

    // Decode the video
    const AVCodec *codec = avcodec_find_decoder(codec_params->codec_id);
    AVCodecContext *codec_context = avcodec_alloc_context3(codec);

    avcodec_parameters_to_context(codec_context, codec_params);
    avcodec_open2(codec_context, codec, nullptr);

    AVFrame *frame = av_frame_alloc();
    AVPacket *packet = av_packet_alloc();

    while (av_read_frame(ifmt_context, packet) >= 0)
    {
        if (packet->stream_index != video_stream_index)
            continue;
        avcodec_send_packet(codec_context, packet);
        int ret = avcodec_receive_frame(codec_context, frame);
        if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
            continue;
        std::cout << "Sucessfully found a frame" << std::endl;
        // save_frame_as_jpeg(codec_context, frame, 1);
        av_packet_unref(packet);
        break;
    }

    // free
    av_frame_free(&frame);
    av_packet_free(&packet);
    avcodec_free_context(&codec_context);
}

Converter::~Converter()
{
    avformat_close_input(&ifmt_context);
    avformat_free_context(ifmt_context);
    avformat_free_context(ofmt_context);
}

void Converter::convert_to_mp4()
{
}