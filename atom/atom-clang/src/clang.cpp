/*
 * Copyright (c) 2016, Joe Roback <joe.roback@gmail.com>. All Rights Reserved.
 */

#include <string>

#include <clang-c/Index.h>

#include <nan.h>

#include "clang_helpers.hpp"
#include "clang_translationunit.hpp"
#include "logger.hpp"


namespace node_clang
{

static void clangDebug(const Nan::FunctionCallbackInfo<v8::Value>& info)
{
    auto local_logging_path = Nan::To<v8::String>(info[0]).ToLocalChecked();
    v8::String::Utf8Value logging_path(local_logging_path);
    this_logger::set_logging_path(*logging_path);
    info.GetReturnValue().Set(Nan::Undefined());
}

static void clangVersion(const Nan::FunctionCallbackInfo<v8::Value>& info)
{
    std::string version = to_string(clang_getClangVersion());
    info.GetReturnValue().Set(Nan::New<v8::String>(version).ToLocalChecked());
}

void initialize(v8::Local<v8::Object> exports)
{
    clang_translationunit::initialize(exports);

    Nan::Set(exports,
             Nan::New<v8::String>("clangDebug").ToLocalChecked(),
             Nan::GetFunction(Nan::New<v8::FunctionTemplate>(clangDebug)).ToLocalChecked());
    Nan::Set(exports,
             Nan::New<v8::String>("clangVersion").ToLocalChecked(),
             Nan::GetFunction(Nan::New<v8::FunctionTemplate>(clangVersion)).ToLocalChecked());
}

NODE_MODULE(clang, initialize)

}
