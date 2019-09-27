#include "file.hpp"


TexoFileExporter::TexoFileExporter(FILE *file): file(file)  {}

void TexoFileExporter::Put(const char c)
{
    fputc(c, file);
}
