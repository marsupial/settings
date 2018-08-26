/*
 * Copyright (c) 2016, Joe Roback <joe.roback@gmail.com>. All Rights Reserved.
 */
#pragma once

#include <string>
#include <utility>
#include <vector>

#include <clang-c/Index.h>


namespace node_clang
{

/**
 * converts a libclang diagnostic to a format better understood by C++/Atom's linter
 */
class diagnostic final
{
public:
    using range_type = std::pair<std::pair<unsigned, unsigned>, std::pair<unsigned, unsigned>>;

public:
    explicit diagnostic(CXDiagnostic cx_diagnostic) noexcept;
    ~diagnostic() noexcept;

    std::string to_string() const& noexcept;

    CXDiagnosticSeverity severity() const& noexcept
    {
        return _severity;
    }

    const char* text() const& noexcept
    {
        return _text.c_str();
    }

    const char* filename() const& noexcept
    {
        return _filename.c_str();
    }

    const range_type& range() const& noexcept
    {
        return _range;
    }

private:
    CXDiagnosticSeverity _severity;
    std::string _text;
    std::string _filename;
    range_type _range;
};

}
