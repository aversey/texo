#ifndef TEXO_INCLUDED_SCRIPT
#define TEXO_INCLUDED_SCRIPT

#include "exporter.hpp"


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Texo String Exporter
 * Simple utility class to make output into C string.
 */
class TexoString: public TexoExporter {
public:
    TexoString(char *&str);


    bool Put(char c);
    bool Put(const char *addon);


private:
    char *&str;
    int    len;
    int    size;
};


#endif
