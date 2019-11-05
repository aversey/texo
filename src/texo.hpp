#ifndef TEXO_INCLUDED_TEXO
#define TEXO_INCLUDED_TEXO

#include <scrvar.hpp>


struct Texo {
    Texo(char c);
    char c;
};


/*
 * Texo Blocks
 */
struct TexoHeader {
    TexoHeader(int level);
    int level;  // Number >= 1, bigger for bigger.
};

struct TexoParagraph {};

struct TexoCode {};

struct TexoQuote {};


/*
 * Texo Decorators
 */
struct TexoMono {};

struct TexoBold {};

struct TexoItalic {};

struct TexoUnderline {};

struct TexoStrike {};


/*
 * Texo Signals
 */
struct TexoImage {
    ScriptVariable path;
    ScriptVariable title;
    ScriptVariable alt;
    ScriptVariable link;
};

struct TexoLink {
    ScriptVariable text;
    ScriptVariable link;
    ScriptVariable title;
};

struct TexoHorizontalRule {};


#endif
