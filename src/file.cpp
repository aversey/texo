#include "file.hpp"

#include <string.h>


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Texo File Exporter
 */
TexoFile::TexoFile(FILE *file): file(file)
{}


bool TexoFile::Put(char c)
{
    fputc(c, file);
    return !ferror(file);
}

bool TexoFile::Put(const char *str)
{
    fwrite(str, 1, strlen(str), file);
    return !ferror(file);
}
