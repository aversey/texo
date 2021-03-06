#include "importer.hpp"


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Texo Importer
 */
TexoImporter::TexoImporter(TexoProducer &producer):
    producer(producer), ok(true)
{}

TexoImporter::~TexoImporter()
{
    End();
}


bool TexoImporter::End()
{
    return ok = ok && TrueEnd();
}

bool TexoImporter::Put(char c)
{
    return ok = ok && TruePut(c);
}

bool TexoImporter::Put(const char *s)
{
    return ok = ok && TruePut(s);
}

bool TexoImporter::Put(FILE *f)
{
    return ok = ok && TruePut(f);
}


bool TexoImporter::TrueEnd()
{
    return producer.End();
}

bool TexoImporter::TruePut(const char *str)
{
    while (*str) {
        ok = TruePut(*str);
        ++str;
    }
    return ok;
}

bool TexoImporter::TruePut(FILE *file)
{
    if (file) {
        for (int c = fgetc(file); ok && c != EOF; c = fgetc(file)) {
            ok = TruePut(c);
        }
    }
    return ok;
}
