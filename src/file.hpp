#ifndef TEXO_INCLUDED_FILE
#define TEXO_INCLUDED_FILE

#include "exporter.hpp"
#include <stdio.h>


class TexoExporterFile: public TexoExporter {
public:
    TexoExporterFile(FILE *file);

    void Put(char c);
    void Put(const ScriptVariable &str);

private:
    FILE *file;
};


#endif
