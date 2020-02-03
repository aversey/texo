#include "script.hpp"

#include <stdlib.h>
#include <string.h>


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Texo String Exporter
 */
TexoString::TexoString(char *&str): str(str)
{
    str  = (char *)malloc(1);
    *str = 0;
    len  = 0;
    size = 1;
}


bool TexoString::Put(char c)
{
    if (len + 1 == size) {
        size *= 2;
        str  = (char *)realloc(str, size);
    }
    if (!str) {
        return false;
    }
    str[len] = c;
    ++len;
    str[len] = 0;
    return true;
}

bool TexoString::Put(const char *addon)
{
    int alen = strlen(addon);
    if (len + alen >= size) {
        size = len + alen + 1;
        str  = (char *)realloc(str, size);
    }
    if (!str) {
        return false;
    }
    memcpy(str + len, addon, alen + 1);
    len += alen;
    return true;
}
