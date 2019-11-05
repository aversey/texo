#ifndef TEXO_INCLUDED_PLAIN
#define TEXO_INCLUDED_PLAIN

#include "exporter.hpp"
#include "producer.hpp"
#include "importer.hpp"


class TexoProducerPlain: public TexoProducer {
public:
    TexoProducerPlain(TexoExporter &exporter);

    void Put(const Texo &piece);

    void Put(const TexoParagraph &piece);
    void Put(const TexoQuote &piece);

    void Put(const TexoHorizontalRule &piece);

private:
    bool quoted;
    bool newline;
};


class TexoImporterPlain: public TexoImporter {
public:
    TexoImporterPlain(TexoProducer &producer);

    void End();

    void Put(char c);
    void Put(const ScriptVariable &str);
    void Put(FILE *file);

private:
    enum State {
        text,
        newline,
        paragraph,
        quote,
        quote_newline,
        rule,
        paragraph_rule
    } state;

    void Text(char c);
    void Newline(char c);
    void Paragraph(char c);
    void Quote(char c);
    void QuoteNewline(char c);
    void Rule(char c);
    void ParagraphRule(char c);

    int dash_count;
};


#endif
