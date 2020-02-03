#ifndef TEXO_INCLUDED_MARKDOWN
#define TEXO_INCLUDED_MARKDOWN

#include "exporter.hpp"
#include "importer.hpp"
#include "producer.hpp"


class TexoProducerMarkdown: public TexoProducer {
public:
    TexoProducerMarkdown(TexoExporter &exporter);

    bool End();

    bool Put(char c);

    bool Code();
    bool Header(int level);
    bool Paragraph();
    bool Quote();

    bool Bold();
    bool Italic();
    bool Mono();
    bool Strike();
    bool Underline();

    bool Link(const char *link, const char *title);
    bool Link();

    bool PutImage(const char *src, const char *alt, const char *title);
    bool PutHorizontalRule();

private:
    bool Mod(const char *str);
    bool Close();
    bool CloseLink();

    bool quoted;
    bool newline;
    bool header;
    bool code;
    bool nospace;

    const char *link_link;
    const char *link_title;
};


class TexoImporterMarkdown: public TexoImporter {
public:
    TexoImporterMarkdown(TexoProducer &producer);


protected:
    bool TruePut(char c);


private:
    enum State {
        start,
        text,
        header_text,
        quote_pre,
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
        header_pre,
        code
    } state, back;
    int header_level;
    int rule_dash_count;
    int code_quote_count;

    bool Start(char c);
    bool Text(char c);
    bool HeaderText(char c);
    bool QuotePre(char c);
    bool QuoteText(char c);
    bool QuoteNewline(char c);
    bool CodeText(char c);
    bool CodeNewline(char c);
    bool CodeEnd(char c);
    bool Backslash(char c);
    bool Asterisk(char c);
    bool Underline(char c);
    bool Plus(char c);
    bool Tilde(char c);
    bool Newline(char c);
    bool Rule(char c);
    bool Paragraph(char c);
    bool Header(char c);
    bool HeaderPre(char c);
    bool Code(char c);

    void Backquote();
};


#endif
