#include "plain.hpp"


TexoPlainProducer::TexoPlainProducer(TexoExporter &exporter):
    TexoProducer(exporter)
{}

void TexoPlainProducer::Put(const Texo &piece)
{
    switch (piece.type) {
    case Texo::character: exporter.Put(piece.c);
    default:              break;
    }
}


TexoPlainImporter::TexoPlainImporter(TexoProducer &producer):
    TexoImporter(producer), first(true), space(0), newline(false)
{}

void TexoPlainImporter::Put(const char c)
{
    if (first) {
        first = false;
        producer.Put(Texo(Texo::paragraph_begin));
    }
    if (c == ' ') {
        ++space;
    } else if (c == '\n') {
        if (newline) {
            newline = false;
            producer.Put(Texo(Texo::paragraph_end));
            producer.Put(Texo(Texo::paragraph_begin));
        } else {
            newline = true;
        }
    } else if (!c) {
        producer.Put(Texo(Texo::paragraph_end));
    } else {
        if (newline) {
            newline = false;
            if (space) {
                space   = 0;
                producer.Put(Texo(Texo::newline));
            } else {
                producer.Put(Texo(' '));
            }
        }
        while (space) {
            producer.Put(Texo(' '));
            --space;
        }
        producer.Put(Texo(c));
    }
}
