#include "file.hpp"


TexoExporterFile::TexoExporterFile(FILE *file): file(file)
{}

void TexoExporterFile::Put(char c)
{
    fputc(c, file);
}

void TexoExporterFile::Put(const ScriptVariable &str)
{
    fwrite(str.c_str(), 1, str.Length(), file);
}
