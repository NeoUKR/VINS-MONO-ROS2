#pragma once

#include <iostream>
#include <streambuf>

class ScopedCeresLogSilencer
{
public:
    explicit ScopedCeresLogSilencer(bool enabled)
        : previous_buffer_(enabled ? std::cerr.rdbuf(&null_buffer_) : nullptr)
    {
    }

    ~ScopedCeresLogSilencer()
    {
        if (previous_buffer_)
            std::cerr.rdbuf(previous_buffer_);
    }

    ScopedCeresLogSilencer(const ScopedCeresLogSilencer &) = delete;
    ScopedCeresLogSilencer &operator=(const ScopedCeresLogSilencer &) = delete;

private:
    class NullBuffer : public std::streambuf
    {
    protected:
        int overflow(int character) override
        {
            return traits_type::not_eof(character);
        }
    };

    NullBuffer null_buffer_;
    std::streambuf *previous_buffer_;
};
