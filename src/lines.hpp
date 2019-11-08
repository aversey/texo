#ifndef TEXO_INCLUDED_LINES
#define TEXO_INCLUDED_LINES

#include "exporter.hpp"
#include "producer.hpp"
#include "importer.hpp"


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Texo Lines Producer
 */
class TexoProducerLines: public TexoProducer {
public:
    TexoProducerLines(TexoExporter &exporter);


    bool End();


    bool Put(char c);


    bool Paragraph();
    bool Quote();


private:
    bool newline;
};


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Texo Lines Importer
 */
class TexoImporterLines: public TexoImporter {
public:
    TexoImporterLines(TexoProducer &producer);


protected:
    bool TruePut(char c);


private:
    bool newline;
    bool quote;
};


#endif
