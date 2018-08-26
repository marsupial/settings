/*
 * Copyright (c) 2016, Joe Roback <joe.roback@gmail.com>. All Rights Reserved.
 */
#pragma once

#include <cassert>
#include <cstdio>

#include <mutex>
#include <string>


namespace node_clang
{

/**
 */
class logger final
{
public:
    /**
     */
    static logger& instance() noexcept
    {
        static logger logger_instance;
        return logger_instance;
    }

    /**
     */
    void set_logging_path(const std::string& logging_path)
    {
        std::unique_lock<std::mutex> guard(_lock);

        // close any existing log file
        close();

        // open file handle for new path
        if (!logging_path.empty())
        {
            _fp = fopen(logging_path.c_str(), "w");
            assert(_fp != nullptr);
        }
    }

    /**
     */
    template <typename... Args>
    void log(Args&&... args) noexcept
    {
        std::unique_lock<std::mutex> guard(_lock);

        if (_fp != nullptr)
        {
            fprintf(_fp, std::forward<Args>(args)...);
            fputc('\n', _fp);
            fflush(_fp);
        }
    }

private:
    explicit logger() noexcept
        : _fp(nullptr)
    {
    }

    ~logger() noexcept
    {
        close();
    }

    /**
     */
    void close() noexcept
    {
        if (_fp != nullptr)
        {
            fclose(_fp);
            _fp = nullptr;
        }
    }

private:
    std::mutex _lock; ///<
    FILE* _fp;        ///<
};

namespace this_logger
{
    /**
     */
    inline void set_logging_path(const std::string& logging_path)
    {
        logger::instance().set_logging_path(logging_path);
    }

    /**
     */
    template <typename... Args>
    inline void log(Args&&... args) noexcept
    {
        logger::instance().log(std::forward<Args>(args)...);
    }
}

}
