#ifndef TEXO_INCLUDED_IMPORTER
#define TEXO_INCLUDED_IMPORTER

#include "producer.hpp"
#include <stdio.h>


class TexoImporter {
public:
    TexoImporter(TexoProducer &producer);

    virtual void End();

    virtual void Put(char c) = 0;
    virtual void Put(const ScriptVariable &str);
    virtual void Put(FILE *file);

protected:
    TexoProducer &producer;
};


#endif
