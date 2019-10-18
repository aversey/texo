#ifndef TEXO_INCLUDED_STR
#define TEXO_INCLUDED_STR

#include "exporter.hpp"


class TexoExporterString: public TexoExporter {
public:
    TexoExporterString(ScriptVariable &str);

    void Put(char c);
    void Put(const ScriptVariable &addon);

private:
    ScriptVariable &str;
};


#endif
