#ifndef TEXO_INCLUDED_PLAIN
#define TEXO_INCLUDED_PLAIN

#include "exporter.hpp"
#include "importer.hpp"
#include "producer.hpp"


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Texo Plain Producer
 */
class TexoProducerPlain: public TexoProducer {
public:
    TexoProducerPlain(TexoExporter &exporter);

    bool End();

    bool Put(char c);

    bool Paragraph();
    bool Quote();

    bool PutHorizontalRule();

private:
    bool quoted;
    bool newline;
    bool nospace;
};


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Texo Plain Importer
 */
class TexoImporterPlain: public TexoImporter {
public:
    TexoImporterPlain(TexoProducer &producer);


protected:
    bool TrueEnd();


    bool TruePut(char c);


private:
    enum State {
        text,
        newline,
        paragraph,
        quote_pre,
        quote,
        quote_newline,
        rule,
        paragraph_rule
    } state;

    bool Text(char c);
    bool Newline(char c);
    bool Paragraph(char c);
    bool QuotePre(char c);
    bool Quote(char c);
    bool QuoteNewline(char c);
    bool Rule(char c);
    bool ParagraphRule(char c);

    int dash_count;
};


#endif
