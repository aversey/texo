#include "importer.hpp"


TexoImporter::TexoImporter(TexoProducer &producer): producer(producer)
{}

void TexoImporter::Put(const ScriptVariable &str)
{
    const int len = str.Length();
    for (int i = 0; i < len; ++i) {
        Put(str[i]);
    }
}

void TexoImporter::Put(FILE *file)
{
    if (file) {
        for (int c = fgetc(file); c != EOF; c = fgetc(file)) {
            Put(c);
        }
    }
}
