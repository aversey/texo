#include "str.hpp"


TexoExporterString::TexoExporterString(ScriptVariable &str): str(str)
{}

void TexoExporterString::Put(char c)
{
    str += c;
}

void TexoExporterString::Put(const ScriptVariable &addon)
{
    str += addon;
}
