/*
 * Copyright (c) 2016, Joe Roback <joe.roback@gmail.com>. All Rights Reserved.
 */
#include "clang_helpers.hpp"


namespace node_clang
{

std::string to_string(CXString cx_string)
{
    std::string ret;
    const char* cstr = clang_getCString(cx_string);

    if (cstr != nullptr)
    {
        ret = cstr;
        clang_disposeString(cx_string);
    }

    return ret;
}

std::string to_string(CXDiagnosticSeverity cx_diagnostic_severity)
{
    std::string ret;

    switch (cx_diagnostic_severity)
    {
        case CXDiagnostic_Note:
            ret = "Info";
            break;
        case CXDiagnostic_Warning:
            ret = "Warning";
            break;
        case CXDiagnostic_Error:
            ret = "Error";
            break;
        case CXDiagnostic_Ignored:
        default:
            ret = "Ignored";
            break;
    }

    return ret;
}

}
