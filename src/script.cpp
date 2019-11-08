#include "script.hpp"


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Texo String Exporter
 */
TexoExporterString::TexoExporterString(ScriptVariable &str): str(str)
{}


bool TexoExporterString::Put(char c)
{
    str += c;
    return true;
}

bool TexoExporterString::Put(const ScriptVariable &addon)
{
    str += addon;
    return true;
}
