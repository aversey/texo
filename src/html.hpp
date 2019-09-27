#ifndef TEXO_INCLUDED_HTML
#define TEXO_INCLUDED_HTML

#include "texo.hpp"


class TexoHTMLProducer: public TexoProducer {
public:
    TexoHTMLProducer(TexoExporter &exporter);
    void Put(const Texo &piece);
};


class TexoHTMLImporter: public TexoImporter {
public:
    TexoHTMLImporter(TexoProducer &producer);
    void Put(const char c);
};


#endif
