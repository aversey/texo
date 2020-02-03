#ifndef TEXO_INCLUDED_IMPORTER
#define TEXO_INCLUDED_IMPORTER

#include "producer.hpp"
#include <stdio.h>


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Texo Importer
 * Texo interface for parsers from input characters into inner representation,
 * described by Texo Producer.
 */
class TexoImporter {
public:
    TexoImporter(TexoProducer &producer);
    ~TexoImporter();


    bool End();

    bool Put(char c);
    bool Put(const char *s);
    bool Put(FILE *f);


protected:
    virtual bool TrueEnd();

    virtual bool TruePut(char c) = 0;
    virtual bool TruePut(const char *str);
    virtual bool TruePut(FILE *file);


    TexoProducer &producer;
    bool          ok;
};


#endif
