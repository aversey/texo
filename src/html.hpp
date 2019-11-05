#ifndef TEXO_INCLUDED_HTML
#define TEXO_INCLUDED_HTML

#include "exporter.hpp"
#include "producer.hpp"
#include "importer.hpp"


class TexoProducerHTML: public TexoProducer {
public:
    TexoProducerHTML(TexoExporter &exporter);

    void End();

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
    void Put(const TexoHorizontalRule &piece);

private:
    void Close();

    enum {
        none = 0,
        header_1,
        header_2,
        header_3,
        header_4,
        header_5,
        header_6,
        paragraph,
        code,
        quote
    } opened_block;
};


class TexoImporterHTML: public TexoImporter {
public:
    TexoImporterHTML(TexoProducer &producer);

    void Put(char c);
    void Put(const ScriptVariable &str);
    void Put(FILE *file);
};


#endif
