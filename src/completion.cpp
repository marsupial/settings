/*
 * Copyright (c) 2016, Joe Roback <joe.roback@gmail.com>. All Rights Reserved.
 */
#include <algorithm>
#include <map>
#include <sstream>

#include "clang_helpers.hpp"

#include "completion.hpp"


namespace node_clang
{

// TODO: add custom autocomplete types that match C++ types..
static std::string kind_type      = "type";
static std::string kind_class     = "class";
static std::string kind_property  = "property";
static std::string kind_function  = "function";
static std::string kind_variable  = "variable";
static std::string kind_macro     = "variable";
static std::string kind_namespace = "value";
static std::string kind_parameter = "value";
static std::string kind_unknown   = "value";

/// maps clang cursor kind to autocomplete plus kind
static std::map<CXCursorKind, std::string> kind_map =
{
    { CXCursor_ClassDecl,               kind_class },
    { CXCursor_ClassTemplate,           kind_class },
    { CXCursor_ObjCInterfaceDecl,       kind_class },
    { CXCursor_ObjCImplementationDecl,  kind_class },
    { CXCursor_FunctionDecl,            kind_function },
    { CXCursor_CXXMethod,               kind_function },
    { CXCursor_FunctionTemplate,        kind_function },
    { CXCursor_ConversionFunction,      kind_function },
    { CXCursor_Constructor,             kind_function },
    { CXCursor_Destructor,              kind_function },
    { CXCursor_ObjCClassMethodDecl,     kind_function },
    { CXCursor_ObjCInstanceMethodDecl,  kind_function },
    { CXCursor_MacroDefinition,         kind_macro },
    { CXCursor_Namespace,               kind_namespace },
    { CXCursor_NamespaceAlias,          kind_namespace },
    { CXCursor_ParmDecl,                kind_parameter },
    { CXCursor_FieldDecl,               kind_property },
    { CXCursor_ObjCIvarDecl,            kind_property },
    { CXCursor_ObjCPropertyDecl,        kind_property },
    { CXCursor_EnumConstantDecl,        kind_property },
    { CXCursor_EnumDecl,                kind_type },
    { CXCursor_StructDecl,              kind_type },
    { CXCursor_UnionDecl,               kind_type },
    { CXCursor_UnexposedDecl,           kind_type },
    { CXCursor_TypedefDecl,             kind_type },
    { CXCursor_VarDecl,                 kind_variable },
};

/// map clang cursor kind to autocomplete-plus type
static std::string kind_to_type(CXCursorKind kind)
{
    auto it = kind_map.find(kind);

    if (it == kind_map.end())
    {
        return kind_unknown;
    }

    return it->second;
}

/// convert clang completion string + chunk into std::string
static std::string chunk_to_string(CXCompletionString completion_string, unsigned chunk)
{
    std::string ret;

    if (completion_string != nullptr)
    {
        CXString cxstring = clang_getCompletionChunkText(completion_string, chunk);
        ret = clang_getCString(cxstring);
        clang_disposeString(cxstring);
    }

    return ret;
}

/// extra any optional completion string from a clang completion string + chunk into std::string
static std::string optional_chunk_to_string(CXCompletionString completion_string, unsigned chunk)
{
    std::string ret;

    if (completion_string != nullptr)
    {
        CXCompletionString optional_completion_string = clang_getCompletionChunkCompletionString(completion_string,
                                                                                                 chunk);

        if (optional_completion_string == nullptr)
        {
            return ret;
        }

        unsigned optional_num_chunks = clang_getNumCompletionChunks(optional_completion_string);

        for (unsigned i = 0; i < optional_num_chunks; ++i)
        {
            CXCompletionChunkKind kind = clang_getCompletionChunkKind(optional_completion_string, i);


            if (kind == CXCompletionChunk_Optional)
            {
                ret += optional_chunk_to_string(optional_completion_string, i);
            }
            else
            {
                ret += chunk_to_string(optional_completion_string, i);
            }
        }
    }

    return ret;
}

completion::completion(const CXCompletionResult& completion_result) noexcept
    : _priority(0)
{
    CXCompletionString completion_string = completion_result.CompletionString;

    // grab internal clang priority
    _priority = clang_getCompletionPriority(completion_string);

    // grab text, displayText, leftLabel, rightLabel autocomplete-plus fields
    unsigned num_chunks = clang_getNumCompletionChunks(completion_string);

    for (unsigned i = 0; i < num_chunks; ++i)
    {
        switch (clang_getCompletionChunkKind(completion_string, i))
        {
            case CXCompletionChunk_ResultType:
            {
                _left_label = chunk_to_string(completion_string, i);
                break;
            }
            case CXCompletionChunk_TypedText:
            {
                _text = chunk_to_string(completion_string, i);
                _display_text += chunk_to_string(completion_string, i);
                break;
            }
            case CXCompletionChunk_Optional:
            {
                _display_text += optional_chunk_to_string(completion_string, i);
                break;
            }
            default:
            {
                _display_text += chunk_to_string(completion_string, i);
                break;
            }
        }
    }

    // find mapping to autocomplete plus kind
    _type = kind_to_type(completion_result.CursorKind);

    // grab any documentation text
    _description = node_clang::to_string(clang_getCompletionBriefComment(completion_string));
}

completion::~completion() noexcept
{
}

std::string completion::to_string() const& noexcept
{
    std::ostringstream os;

    os << "{ ";
    os << "prio=" << _priority << ", ";
    os << "type=" << _type << ", ";
    os << "text=" << _text << ", ";
    os << "left=" << _left_label << ", ";
    os << "display=" << _display_text << ", ";
    os << "right=" << _right_label << ", ";
    os << "desc=" << _description;
    os << " }";

    return os.str();
}

}
