/*
 * Copyright (c) 2016, Joe Roback <joe.roback@gmail.com>. All Rights Reserved.
 */
#pragma once

#include <cstdint>
#include <vector>

#include <clang-c/Index.h>

#include <nan.h>

#include "defines.hpp"


namespace node_clang
{

/**
 * C++ wrapper to convert Javascript array of unsaved file data
 */
class unsaved_files final
{
    NODE_CLANG_NO_COPYABLE(unsaved_files);
    NODE_CLANG_NO_MOVEABLE(unsaved_files);

public:
    explicit unsaved_files(const v8::Local<v8::Array>& js_unsaved_files);
    ~unsaved_files() noexcept;

    /**
     * @return pointer to CXUnsavedFile data
     */
    CXUnsavedFile* data() & noexcept
    {
        return _data.data();
    }

    /**
     * @return number of unsaved files
     */
    std::size_t size() const& noexcept
    {
        return _data.size();
    }

    /**
     * optimization to dispose of unsaved file data in the libuv thread
     */
    void dispose() & noexcept;

private:
    std::vector<CXUnsavedFile> _data;  ///< unsaved file data
};

}
