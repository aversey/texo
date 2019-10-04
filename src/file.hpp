#ifndef TEXO_INCLUDED_FILE
#define TEXO_INCLUDED_FILE

#include "texo.hpp"


class TexoFileExporter: public TexoExporter {
public:
    TexoFileExporter(FILE *file);
    void Put(const char c);

private:
    FILE *file;
};


#endif
