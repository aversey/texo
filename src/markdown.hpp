#ifndef TEXO_INCLUDED_MARKDOWN
#define TEXO_INCLUDED_MARKDOWN

#include "exporter.hpp"
#include "producer.hpp"
#include "importer.hpp"


class TexoProducerMarkdown: public TexoProducer {
public:
    TexoProducerMarkdown(TexoExporter &exporter);

    void Put(const Texo &piece);

    void Put(const TexoHeader &piece);
    void Put(const TexoParagraph &piece);
    void Put(const TexoCode &piece);
    void Put(const TexoQuote &piece);

    void Put(const TexoMono &piece);
    void Put(const TexoBold &piece);
    void Put(const TexoItalic &piece);
    void Put(const TexoUnderline &piece);
    void Put(const TexoStrike &piece);

    void Put(const TexoImage &piece);
    void Put(const TexoLink &piece);
    void Put(const TexoLineBreak &piece);
    void Put(const TexoHorizontalRule &piece);

private:
    bool quoted;
    bool newline;
    bool header;
    bool code;
};


class TexoImporterMarkdown: public TexoImporter {
public:
    TexoImporterMarkdown(TexoProducer &producer);

    void Put(char c);
    void Put(const ScriptVariable &str);
    void Put(FILE *file);

private:
    enum State {
        text,
        header_text,
        quote_text,
        quote_newline,
        code_text,
        code_newline,
        code_end,
        backslash,
        asterisk,
        underline,
        plus,
        tilde,
        newline,
        rule,
        paragraph,
        header,
        code,
        quote
    } state;
    State wrapping_state;
    bool is_italic;
    bool is_bold;
    bool is_underline;
    bool is_strike;
    bool is_mono;
    int header_level;
    int rule_dash_count;
    int code_quote_count;

    void Text(char c);
    void HeaderText(char c);
    void QuoteText(char c);
    void QuoteNewline(char c);
    void CodeText(char c);
    void CodeNewline(char c);
    void CodeEnd(char c);
    void Backslash(char c);
    void Asterisk(char c);
    void Underline(char c);
    void Plus(char c);
    void Tilde(char c);
    void Newline(char c);
    void Rule(char c);
    void Paragraph(char c);
    void Header(char c);
    void Code(char c);
    void Quote(char c);

    void Backquote();
    void EndHeader();
};


#endif
