#include "texo.hpp"


Texo::Texo(Type type):
    type(type), c(0), image_src(0), image_alt(0), link_url(0)
{}

Texo::Texo(const char c):
    type(character), c(c), image_src(0), image_alt(0), link_url(0)
{}


void TexoExporter::PutStr(const char *str)
{
    if (str) {
        while (*str) {
            Put(*str);
            ++str;
        }
    }
}


TexoProducer::TexoProducer(TexoExporter &exporter): exporter(exporter)  {}

void TexoProducer::PutStr(const char *str)
{
    if (str) {
        while (*str) {
            Put(Texo(*str));
            ++str;
        }
    }
}


TexoImporter::TexoImporter(TexoProducer &producer): producer(producer)  {}

void TexoImporter::PutStr(const char *str)
{
    if (str) {
        while (*str) {
            Put(*str);
            ++str;
        }
        Put(0);
    }
}

void TexoImporter::PutFile(FILE *file)
{
    if (file) {
        for (int c = fgetc(file); c != EOF; c = fgetc(file)) {
            Put(c);
        }
        Put(0);
    }
}
