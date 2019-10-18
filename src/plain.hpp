#ifndef TEXO_INCLUDED_PLAIN
#define TEXO_INCLUDED_PLAIN

#include "exporter.hpp"
#include "producer.hpp"
#include "importer.hpp"


class TexoProducerPlain: public TexoProducer {
public:
    TexoProducerPlain(TexoExporter &exporter);

    void Put(const Texo &piece);

    void Put(const TexoHeader &piece);
    void Put(const TexoParagraph &piece);
    void Put(const TexoQuote &piece);

    void Put(const TexoLineBreak &piece);
    void Put(const TexoHorizontalRule &piece);

private:
    bool quoted;
};


class TexoImporterPlain: public TexoImporter {
public:
    TexoImporterPlain(TexoProducer &producer);
    ~TexoImporterPlain();

    void Put(char c);
    void Put(const ScriptVariable &str);

private:
    enum State {
        text,
        space,
        newline,
        paragraph,
        quote,
        quote_newline,
        header
    } state;

    void Text(char c);
    void Space(char c);
    void Newline(char c);
    void Paragraph(char c);
    void Quote(char c);
    void QuoteNewline(char c);
    void Header(char c);
};


#endif
