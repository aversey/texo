#include "html.hpp"


TexoHTMLProducer::TexoHTMLProducer(TexoExporter &exporter):
    TexoProducer(exporter)
{}

void TexoHTMLProducer::Put(const Texo &piece)
{
    switch (piece.type) {
    case Texo::character:       exporter.Put(piece.c);    break;
    case Texo::paragraph_begin: exporter.PutStr("<p>");   break;
    case Texo::paragraph_end:   exporter.PutStr("</p>");  break;
    case Texo::newline:         exporter.PutStr("<br/>");
    default:                    break;
    }
}


TexoHTMLImporter::TexoHTMLImporter(TexoProducer &producer):
    TexoImporter(producer)
{}

void TexoHTMLImporter::Put(const char c)
{
    producer.Put(Texo(c));
}
