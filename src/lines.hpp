#ifndef TEXO_INCLUDED_LINES
#define TEXO_INCLUDED_LINES

#include "exporter.hpp"
#include "producer.hpp"
#include "importer.hpp"


class TexoProducerLines: public TexoProducer {
public:
    TexoProducerLines(TexoExporter &exporter);

    void Put(const Texo &piece);

    void Put(const TexoParagraph &piece);
    void Put(const TexoQuote &piece);

private:
    bool newline;
};


class TexoImporterLines: public TexoImporter {
public:
    TexoImporterLines(TexoProducer &producer);

    void Put(char c);
    void Put(const ScriptVariable &str);
    void Put(FILE *file);

private:
    bool newline;
};


#endif
