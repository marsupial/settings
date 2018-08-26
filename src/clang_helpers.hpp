/*
 * Copyright (c) 2016, Joe Roback <joe.roback@gmail.com>. All Rights Reserved.
 */
#pragma once

#include <string>

#include <clang-c/Index.h>


namespace node_clang
{

/// convert clang string to std::string
std::string to_string(CXString cx_string);

/// convert clang diagnostic severity to std::string
std::string to_string(CXDiagnosticSeverity cx_diagnostic_severity);

}
