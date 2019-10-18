#include "exporter.hpp"


void TexoExporter::Put(const ScriptVariable &str)
{
    const int len = str.Length();
    for (int i = 0; i < len; ++i) {
        Put(str[i]);
    }
}
