// ffmpeg_job.hpp
#pragma once

#include "unix_command.hpp"

struct FFmpegJob
{
    std::string input;
    std::string output;

    std::optional<std::string> videoCodec;
    std::optional<std::string> audioCodec;
    std::optional<int> videoBitrateK;
    std::optional<int> audioBitrateK;
    std::optional<int> videoFramerate;
    std::optional<std::string> format;

    std::vector<std::string> extraArgs;
};

template <StringOutputIterator Out>
void make_args (const FFmpegJob& job, Out out)
{
    *out++ = "-y";

    *out++ = "-i";
    *out++ = job.input;

    if (job.videoCodec)
    {
        *out++ = "-c:v";
        *out++ = *job.videoCodec;
    }

    if (job.videoBitrateK)
    {
        *out++ = "-b:v";
        *out++ = std::to_string (*job.videoBitrateK) + "k";
    }

    if (job.videoFramerate)
    {
        *out++ = "-r";
        *out++ = std::to_string (*job.videoFramerate);
    }

    if (job.audioCodec)
    {
        *out++ = "-c:a";
        *out++ = *job.audioCodec;
    }

    if (job.audioBitrateK)
    {
        *out++ = "-b:a";
        *out++ = std::to_string (*job.audioBitrateK) + "k";
    }

    if (job.format)
    {
        *out++ = "-f";
        *out++ = *job.format;
    }

    append_args (out, job.extraArgs);

    *out++ = job.output;
}

inline UnixCommand make_ffmpeg_command (
    const FFmpegJob& job,
    const std::string& ffmpegBinary = "ffmpeg"
)
{
    return make_unix_command (ffmpegBinary, job);
}
