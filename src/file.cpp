#include "file.hpp"


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Texo File Exporter
 */
TexoExporterFile::TexoExporterFile(FILE * file): file(file)
{}


bool TexoExporterFile::Put(char c)
{
    fputc(c, file);
    return !ferror(file);
}

bool TexoExporterFile::Put(const ScriptVariable & str)
{
    fwrite(str.c_str(), 1, str.Length(), file);
    return !ferror(file);
}
