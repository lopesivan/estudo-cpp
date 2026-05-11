#include "ffmpeg_job.hpp"

int main()
{
    FFmpegJob job;
    job.input = "entrada.mp3";
    job.output = "saida.ogg";
    job.audioCodec = "libvorbis";
    job.audioBitrateK = 192;

    UnixCommand cmd = make_ffmpeg_command (job);

    return cmd.run();
}
