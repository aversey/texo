#ifndef TEXO_INCLUDED_EXPORTER
#define TEXO_INCLUDED_EXPORTER


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Texo Exporter
 * Texo interface for outputting.
 */
class TexoExporter {
public:
    virtual bool Put(char c) = 0;
    virtual bool Put(const char *str);
};


#endif
