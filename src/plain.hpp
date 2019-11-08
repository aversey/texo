#ifndef TEXO_INCLUDED_PLAIN
#define TEXO_INCLUDED_PLAIN

#include "exporter.hpp"
#include "producer.hpp"
#include "importer.hpp"


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

    void Text(char c);
    void Newline(char c);
    void Paragraph(char c);
    void QuotePre(char c);
    void Quote(char c);
    void QuoteNewline(char c);
    void Rule(char c);
    void ParagraphRule(char c);

    int dash_count;
};


#endif
