#ifndef TEXO_INCLUDED_FILE
#define TEXO_INCLUDED_FILE

#include "exporter.hpp"
#include <stdio.h>


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Texo File Exporter
 * Simple utility class to make output into FILE *variable.
 */
class TexoExporterFile: public TexoExporter {
public:
    TexoExporterFile(FILE * file);


    bool Put(char c);
    bool Put(const ScriptVariable & str);


private:
    FILE * file;
};


#endif
