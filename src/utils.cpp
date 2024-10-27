// Implementations of utility functions

#include "utils.hpp"
#include <filesystem>

const char *get_file_path_from_argv(const int argc, const char *argv[])
{
    if (argc < 2)
    {
        std::cerr << "Usage: " << argv[0] << " <video_file>" << std::endl;
        exit(EXIT_FAILURE);
    }
    return argv[1];
}

std::tuple<std::string, std::string> get_filename_and_ext(std::string input_file_path)
{
    /*
    Extracts filename and extension from filepath and returns as tuple.
    */
    std::filesystem::path p = input_file_path;
    auto file = p.filename();
    return {file.stem(), file.extension()};
}

int save_frame_as_jpeg(AVCodecContext *pCodecCtx, AVFrame *pFrame, int FrameNo)
{
    int ret = 0;

    const AVCodec *jpegCodec = avcodec_find_encoder(AV_CODEC_ID_MJPEG);
    if (!jpegCodec)
    {
        return -1;
    }
    AVCodecContext *jpegContext = avcodec_alloc_context3(jpegCodec);
    if (!jpegContext)
    {
        return -1;
    }

    jpegContext->pix_fmt = pCodecCtx->pix_fmt;
    jpegContext->height = pFrame->height;
    jpegContext->width = pFrame->width;
    jpegContext->time_base = AVRational{1, 10};

    jpegContext->strict_std_compliance = FF_COMPLIANCE_UNOFFICIAL;
    ret = avcodec_open2(jpegContext, jpegCodec, NULL);
    if (ret < 0)
    {
        return ret;
    }
    FILE *JPEGFile;
    char JPEGFName[256];

    AVPacket *packet = av_packet_alloc();

    int gotFrame;

    ret = avcodec_send_frame(jpegContext, pFrame);
    if (ret < 0)
    {
        return ret;
    }

    ret = avcodec_receive_packet(jpegContext, packet);
    if (ret < 0)
    {
        return ret;
    }

    sprintf(JPEGFName, "frame-%06d.jpg", FrameNo);
    JPEGFile = fopen(JPEGFName, "wb");
    fwrite(packet->data, 1, packet->size, JPEGFile);
    fclose(JPEGFile);

    av_packet_unref(packet);
    avcodec_close(jpegContext);
    return 0;
}