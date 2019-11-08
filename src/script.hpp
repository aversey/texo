#ifndef TEXO_INCLUDED_SCRIPT
#define TEXO_INCLUDED_SCRIPT

#include "exporter.hpp"

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Texo String Exporter
 * Simple utility class to make output into Script Variable,
 * which can be easily transformed into C string via c_str method.
 */
class TexoExporterString: public TexoExporter {
public:
    TexoExporterString(ScriptVariable &str);


    bool Put(char c);
    bool Put(const ScriptVariable &addon);


private:
    ScriptVariable &str;
};


#endif
