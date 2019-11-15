#ifndef TEXO_INCLUDED_MARKDOWN
#define TEXO_INCLUDED_MARKDOWN

#include "exporter.hpp"
#include "importer.hpp"
#include "producer.hpp"


class TexoProducerMarkdown: public TexoProducer {
public:
    TexoProducerMarkdown(TexoExporter & exporter);

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

    bool Link(const ScriptVariable & link, const ScriptVariable & title);
    bool Link();

    bool PutImage(const ScriptVariable & src,
                  const ScriptVariable & alt,
                  const ScriptVariable & title);
    bool PutHorizontalRule();

private:
    bool Mod(const ScriptVariable & str);
    bool Close();
    bool CloseLink();

    bool quoted;
    bool newline;
    bool header;
    bool code;
    bool nospace;

    ScriptVariable link_link;
    ScriptVariable link_title;
};


class TexoImporterMarkdown: public TexoImporter {
public:
    TexoImporterMarkdown(TexoProducer & producer);


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
    } state,
        back;
    int header_level;
    int rule_dash_count;
    int code_quote_count;

    void Start(char c);
    void Text(char c);
    void HeaderText(char c);
    void QuotePre(char c);
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
    void HeaderPre(char c);
    void Code(char c);

    void Backquote();
};


#endif
