/*
 * Copyright (c) 2016, Joe Roback <joe.roback@gmail.com>. All Rights Reserved.
 */
#pragma once

#include <cstdint>
#include <vector>

#include <nan.h>

#include "defines.hpp"


namespace node_clang
{

/**
 * converts a javascript array of command line arguments into a C++ vector
 * to that libclang can have something it understands
 */
class command_line_args final
{
    NODE_CLANG_NO_COPYABLE(command_line_args);
    NODE_CLANG_NO_MOVEABLE(command_line_args);

public:
    explicit command_line_args() noexcept;
    explicit command_line_args(const v8::Local<v8::Array>& js_command_line_args) noexcept;
    ~command_line_args() noexcept;

    const char* const* data() const& noexcept
    {
        return _data.data();
    }

    std::size_t size() const& noexcept
    {
        return _data.size();
    }

    /**
     * a pure optimization so that the user can deallocate
     * the storage associated with the arguments early, like say on a libuv thread
     * instead of the main javascript event loop thread
     */
    void dispose() & noexcept;

private:
    std::vector<const char*> _data;
};

}
