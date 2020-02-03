#ifndef TEXO_INCLUDED_FILE
#define TEXO_INCLUDED_FILE

#include "exporter.hpp"
#include <stdio.h>


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Texo File Exporter
 * Simple utility class to make output into FILE *variable.
 */
class TexoFile: public TexoExporter {
public:
    TexoFile(FILE *file);


    bool Put(char c);
    bool Put(const char *str);


private:
    FILE *file;
};


#endif
