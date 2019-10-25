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
};


#endif
