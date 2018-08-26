/*
 * Copyright (c) 2016, Joe Roback <joe.roback@gmail.com>. All Rights Reserved.
 */
#include "unsaved_files.hpp"


namespace node_clang
{

unsaved_files::unsaved_files(const v8::Local<v8::Array>& js_unsaved_files)
{
    std::uint32_t length = js_unsaved_files->Length();
    _data.reserve(length);

    v8::Local<v8::String> filename_prop = Nan::New<v8::String>("filename").ToLocalChecked();
    v8::Local<v8::String> contents_prop = Nan::New<v8::String>("contents").ToLocalChecked();

    // having to copy all the editor data is not ideal, but its not the worse thing
    // and such is life when performing javascript operations outside the main event loop..

    for (std::uint32_t i = 0; i < length; ++i)
    {
        if (Nan::Has(js_unsaved_files, i).FromJust())
        {
            v8::Local<v8::Object> element = Nan::Get(js_unsaved_files, i).ToLocalChecked()->ToObject();
            auto filename = Nan::Get(element, filename_prop).ToLocalChecked()->ToString();
            auto contents = Nan::Get(element, contents_prop).ToLocalChecked()->ToString();

            std::size_t filename_length = filename->Utf8Length();
            char* filename_data = new char[filename_length + 1];
            Nan::DecodeWrite(filename_data, filename_length, filename, Nan::UTF8);
            filename_data[filename_length] = '\0';

            std::size_t contents_length = contents->Utf8Length();
            char* contents_data = new char[contents_length + 1];
            Nan::DecodeWrite(contents_data, contents_length, contents, Nan::UTF8);
            contents_data[contents_length] = '\0';

            _data.emplace_back();
            auto& dit = _data.back();
            dit.Filename = filename_data;
            dit.Contents = contents_data;
            dit.Length = contents_length;
        }
    }
}

unsaved_files::~unsaved_files() noexcept
{
    dispose();
}

void unsaved_files::dispose() & noexcept
{
    if (!_data.empty())
    {
        for (auto& unsaved_file : _data)
        {
            delete unsaved_file.Filename;
            delete unsaved_file.Contents;
        }

        _data.clear();
        _data.shrink_to_fit();
    }
}

}
