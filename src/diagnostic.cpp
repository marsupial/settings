/*
 * Copyright (c) 2016, Joe Roback <joe.roback@gmail.com>. All Rights Reserved.
 */
#include <cstdio>
#include <sstream>

#include "clang_helpers.hpp"

#include "diagnostic.hpp"


namespace node_clang
{

diagnostic::diagnostic(CXDiagnostic cx_diagnostic) noexcept
    : _severity(clang_getDiagnosticSeverity(cx_diagnostic))
    , _text(node_clang::to_string(clang_getDiagnosticSpelling(cx_diagnostic)))
{
    auto location = clang_getDiagnosticLocation(cx_diagnostic);

    CXFile file;
    unsigned location_line;
    unsigned location_column;
    clang_getExpansionLocation(location, &file, &location_line, &location_column, nullptr);

    _filename = node_clang::to_string(clang_getFileName(file));

    // atom linter only support one range, so if one range is available use that, otherwise fallback to location
    unsigned num_ranges = clang_getDiagnosticNumRanges(cx_diagnostic);

    if (num_ranges == 1)
    {
        auto source_range = clang_getDiagnosticRange(cx_diagnostic, 0);

        auto location_start = clang_getRangeStart(source_range);
        unsigned start_line;
        unsigned start_column;
        clang_getExpansionLocation(location_start, nullptr, &start_line, &start_column, nullptr);

        auto location_end = clang_getRangeEnd(source_range);
        unsigned end_line;
        unsigned end_column;
        clang_getExpansionLocation(location_end, nullptr, &end_line, &end_column, nullptr);

        _range.first.first   = start_line - 1;
        _range.first.second  = start_column - 1;
        _range.second.first  = end_line - 1;
        _range.second.second = end_column - 1;
    }
    else
    {
        _range.first.first   = location_line - 1;
        _range.first.second  = location_column - 1;
        _range.second.first  = location_line - 1;
        _range.second.second = location_column - 1;
    }

    clang_disposeDiagnostic(cx_diagnostic);
}

diagnostic::~diagnostic() noexcept
{
}

std::string diagnostic::to_string() const& noexcept
{
    std::ostringstream os;

    os << "{ ";
    os << "severity=" << node_clang::to_string(_severity) << ", ";
    os << "text=" << _text << ", ";
    os << "filename=" << _filename << ", ";
    os << "range=[ ";
    os << _range.first.first << ", ";
    os << _range.first.second << ", ";
    os << _range.second.first << ", ";
    os << _range.second.second << " ]";
    os << " }";

    return os.str();
}

}
