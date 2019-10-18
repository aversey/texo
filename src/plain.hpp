#ifndef TEXO_INCLUDED_PLAIN
#define TEXO_INCLUDED_PLAIN

#include "exporter.hpp"
#include "producer.hpp"
#include "importer.hpp"
#include "texo.hpp"


class TexoProducerPlain: public TexoProducer {
public:
    TexoProducerPlain(TexoExporter &exporter);
};


class TexoImporterPlain: public TexoImporter {
public:
    TexoImporterPlain(TexoProducer &producer);
};


#endif
