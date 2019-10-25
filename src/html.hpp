#ifndef TEXO_INCLUDED_HTML
#define TEXO_INCLUDED_HTML

#include "exporter.hpp"
#include "producer.hpp"
#include "importer.hpp"


class TexoProducerHTML: public TexoProducer {
public:
    TexoProducerHTML(TexoExporter &exporter);

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
};


class TexoImporterHTML: public TexoImporter {
public:
    TexoImporterHTML(TexoProducer &producer);

    void Put(char c);
    void Put(const ScriptVariable &str);
    void Put(FILE *file);

private:
    enum State {
        text, // TODO: decoding ("&gt;" -> ">")
        open,
        tag_name,
        tag_skip,
        attr_pre,
        attr_name,
        attr_name_post,
        attr_value_pre,
        attr_value
    } state;
    bool           closing;
    ScriptVariable tag;
    ScriptVariable attr;
    ScriptVariable attr_src;
    ScriptVariable attr_href;
    ScriptVariable attr_title;
    ScriptVariable attr_alt;
    ScriptVariable link_text;
};


#endif
