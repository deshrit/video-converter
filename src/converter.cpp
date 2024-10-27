#include "converter.hpp"
#include "utils.hpp"

Converter::Converter(std::string file_path)
{
    this->file_path = file_path;
    this->ifmt_context = this->ofmt_context = nullptr;
    this->video_stream_index = this->audio_stream_index = -1;
    this->video_codec_params = this->audio_codec_params = nullptr;

    this->load_video_file();
    // this->dump_video_info();
    this->remux_to_mp4();
}

Converter::~Converter()
{
    avformat_close_input(&this->ifmt_context);
    avformat_free_context(this->ifmt_context);
    avformat_free_context(this->ofmt_context);
}

void Converter::load_video_file()
{
    // Load video file
    int ret = avformat_open_input(&this->ifmt_context, this->file_path.c_str(), nullptr, nullptr);
    if (ret != 0)
    {
        std::cerr << "Unable to open '" << file_path << "' error code " << ret << std::endl;
        exit(EXIT_FAILURE);
    }
    ret = avformat_find_stream_info(this->ifmt_context, nullptr);
    if (ret != 0)
    {
        std::cerr << "Unable to find stream info on '" << file_path << "' error code " << ret << std::endl;
        exit(EXIT_FAILURE);
    }

    // Set stream indexes
    set_video_stream_index();
    set_audio_stream_index();
}

void Converter::set_video_stream_index()
{
    for (int i = 0; i < this->ifmt_context->nb_streams; i++)
    {
        auto stream = this->ifmt_context->streams[i];
        video_codec_params = stream->codecpar;
        if (video_codec_params->codec_type == AVMEDIA_TYPE_VIDEO)
        {
            video_stream_index = 1;
            return;
        }
    }
    video_codec_params = nullptr;
    std::cout << "Unable to find video stream in '" << file_path << "'\n";
    exit(EXIT_FAILURE);
}

void Converter::set_audio_stream_index()
{
    for (int i = 0; i < this->ifmt_context->nb_streams; i++)
    {
        auto stream = this->ifmt_context->streams[i];
        audio_codec_params = stream->codecpar;
        if (audio_codec_params->codec_type == AVMEDIA_TYPE_VIDEO)
        {
            audio_stream_index = 1;
            return;
        }
    }
    audio_codec_params = nullptr;
    std::cout << "Unable to find audio stream in '" << file_path << "'\n";
}

void Converter::dump_video_info()
{
    av_dump_format(this->ifmt_context, 0, file_path.c_str(), 0);
}

void Converter::remux_to_mp4()
{
    auto [filename, ext] = get_filename_and_ext(this->file_path);
    std::string out_file_name = filename + "_converted.mp4";
    const AVOutputFormat *ofmt = nullptr;
    AVPacket *pkt = nullptr;
    pkt = av_packet_alloc();
    int ret, i;
    int stream_index = 0;
    int *stream_mapping = nullptr;
    int stream_mapping_size = 0;

    avformat_alloc_output_context2(&this->ofmt_context, nullptr, nullptr, out_file_name.c_str());

    stream_mapping_size = this->ifmt_context->nb_streams;
    stream_mapping = (int *)av_calloc(stream_mapping_size, sizeof(*stream_mapping));
    ofmt = this->ofmt_context->oformat;

    for (int i = 0; i < this->ifmt_context->nb_streams; i++)
    {
        AVStream *out_stream = nullptr;
        AVStream *in_stream = this->ifmt_context->streams[i];
        AVCodecParameters *in_codecpar = in_stream->codecpar;

        if (in_codecpar->codec_type != AVMEDIA_TYPE_AUDIO &&
            in_codecpar->codec_type != AVMEDIA_TYPE_VIDEO &&
            in_codecpar->codec_type != AVMEDIA_TYPE_SUBTITLE)
        {
            stream_mapping[i] = -1;
            continue;
        }

        stream_mapping[i] = stream_index++;

        out_stream = avformat_new_stream(this->ofmt_context, nullptr);

        avcodec_parameters_copy(out_stream->codecpar, in_codecpar);
        out_stream->codecpar->codec_tag = 0;
    }

    // av_dump_format(this->ofmt_context, 0, out_file_name.c_str(), 1);

    avio_open(&this->ofmt_context->pb, out_file_name.c_str(), AVIO_FLAG_WRITE);
    ret = avformat_write_header(this->ofmt_context, nullptr);
    if (ret < 0)
    {
        std::cerr << "Error occured while opening output file\n";
        return;
    }

    while (1)
    {
        AVStream *in_stream = nullptr, *out_stream = nullptr;
        int ret = av_read_frame(this->ifmt_context, pkt);
        if (ret < 0)
            break;

        in_stream = this->ifmt_context->streams[pkt->stream_index];
        if (pkt->stream_index >= stream_mapping_size ||
            stream_mapping[pkt->stream_index] < 0)
        {
            av_packet_unref(pkt);
            continue;
        }

        pkt->stream_index = stream_mapping[pkt->stream_index];
        out_stream = this->ofmt_context->streams[pkt->stream_index];

        av_packet_rescale_ts(pkt, in_stream->time_base, out_stream->time_base);
        pkt->pos = -1;
        ret = av_interleaved_write_frame(this->ofmt_context, pkt);
        if (ret < 0)
        {
            std::cerr << "Error muxing packets\n";
            break;
        }
    }

    av_write_trailer(this->ofmt_context);
}