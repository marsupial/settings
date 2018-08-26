/*
 * Copyright (c) 2016, Joe Roback <joe.roback@gmail.com>. All Rights Reserved.
 */
#include <cstdio>
#include <string>

#include "command_line_args.hpp"


namespace node_clang
{

command_line_args::command_line_args() noexcept
{
}

command_line_args::command_line_args(const v8::Local<v8::Array>& js_command_line_args) noexcept
{
    std::uint32_t length = js_command_line_args->Length();
    _data.reserve(length + 1);

    for (std::uint32_t i = 0; i < length; ++i)
    {
        if (Nan::Has(js_command_line_args, i).FromJust())
        {
            auto element = Nan::Get(js_command_line_args, i).ToLocalChecked()->ToString();
            std::size_t element_length = element->Utf8Length();

            char* element_data = new char[element_length + 1];
            Nan::DecodeWrite(element_data, element_length, element, Nan::UTF8);
            element_data[element_length] = '\0';

            _data.emplace_back(element_data);
        }
    }

    // workaround for missing search path using libclang on linux (macport clang seems to get it right...)
    std::size_t workaround_length = strlen(CLANG_SEARCH_PATH) + 3;
    char* workaround = new char[workaround_length];
    std::snprintf(workaround, workaround_length, "-I%s", CLANG_SEARCH_PATH);
    _data.emplace_back(workaround);
}

command_line_args::~command_line_args() noexcept
{
    dispose();
}

void command_line_args::dispose() & noexcept
{
    if (!_data.empty())
    {
        for (auto& command_line_arg : _data)
        {
            delete command_line_arg;
        }

        _data.clear();
        _data.shrink_to_fit();
    }
}

}
