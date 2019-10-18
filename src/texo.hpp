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
    TexoHeader(int level, bool closing = false);
    int  level; // Number >= 1, bigger for bigger.
    bool closing;
};

struct TexoParagraph {
    TexoParagraph(bool closing = false);
    bool closing;
};

struct TexoCode {
    TexoCode(bool closing = false);
    bool closing;
};

struct TexoQuote {
    TexoQuote(bool closing = false);
    bool closing;
};

/*
 * Texo Decorators
 */
struct TexoMono {
    TexoMono(bool closing = false);
    bool closing;
};

struct TexoBold {
    TexoBold(bool closing = false);
    bool closing;
};

struct TexoItalic {
    TexoItalic(bool closing = false);
    bool closing;
};

struct TexoUnderline {
    TexoUnderline(bool closing = false);
    bool closing;
};

struct TexoStrike {
    TexoStrike(bool closing = false);
    bool closing;
};

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

struct TexoLineBreak {
};

struct TexoHorizontalRule {
};


#endif
