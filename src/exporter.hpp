#ifndef TEXO_INCLUDED_EXPORTER
#define TEXO_INCLUDED_EXPORTER

#include <scrvar.hpp>


class TexoExporter {
public:
    virtual void Put(char c) = 0;
    virtual void Put(const ScriptVariable &str);
};


#endif
