#include "exporter.hpp"

#include <string.h>


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Texo Exporter
 */
bool TexoExporter::Put(const char *str)
{
    bool ok = true;
    while (ok && *str) {
        ok = Put(*str);
        ++str;
    }
    return ok;
}
