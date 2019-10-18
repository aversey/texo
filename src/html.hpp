#ifndef TEXO_INCLUDED_HTML
#define TEXO_INCLUDED_HTML

#include "exporter.hpp"
#include "producer.hpp"
#include "importer.hpp"
#include "texo.hpp"


class TexoProducerHTML: public TexoProducer {
public:
    TexoProducerHTML(TexoExporter &exporter);
};


class TexoImporterHTML: public TexoImporter {
public:
    TexoImporterHTML(TexoProducer &producer);
};


#endif
