/*
 * Copyright (c) 2016, Joe Roback <joe.roback@gmail.com>. All Rights Reserved.
 */
#pragma once

#include <string>

#include <clang-c/Index.h>


namespace node_clang
{

/**
 * converts a clang completion object and all its children into a C++
 * object for final consumption by autocomplete-plus in javascript
 */
class completion final
{
public:
    explicit completion(const CXCompletionResult& completion_result) noexcept;
    ~completion() noexcept;

    std::string to_string() const& noexcept;

    unsigned priority() const& noexcept
    {
        return _priority;
    }

    const char* text() const& noexcept
    {
        return _text.c_str();
    }

    const char* display_text() const& noexcept
    {
        return _display_text.c_str();
    }

    const char* type() const& noexcept
    {
        return _type.c_str();
    }

    const char* left_label() const& noexcept
    {
        return _left_label.c_str();
    }

    const char* right_label() const& noexcept
    {
        return _right_label.c_str();
    }

    const char* description() const& noexcept
    {
        return _description.c_str();
    }

private:
    unsigned _priority;
    std::string _text;
    std::string _display_text;
    std::string _type;
    std::string _left_label;
    std::string _right_label;
    std::string _description;
};

}
