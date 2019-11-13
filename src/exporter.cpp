#include "exporter.hpp"


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Texo Exporter
 */
bool TexoExporter::Put(const ScriptVariable & str)
{
    bool      ok  = true;
    const int len = str.Length();
    for (int i = 0; ok && i < len; ++i) {
        ok = Put(str[i]);
    }
    return ok;
}
