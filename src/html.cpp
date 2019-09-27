#include "html.hpp"


TexoHTMLProducer::TexoHTMLProducer(TexoExporter &exporter):
    TexoProducer(exporter)
{}

void TexoHTMLProducer::Put(const Texo &piece)
{
    switch (piece.type) {
    case Texo::character:       exporter.Put(piece.c);     break;
    case Texo::paragraph_begin: exporter.PutStr("<p>");    break;
    case Texo::paragraph_end:   exporter.PutStr("</p>");   break;
    case Texo::newline:         exporter.PutStr("<br/>");  break;
    case Texo::bold_begin:      exporter.PutStr("<b>");    break;
    case Texo::bold_end:        exporter.PutStr("</b>");   break;
    case Texo::italic_begin:    exporter.PutStr("<i>");    break;
    case Texo::italic_end:      exporter.PutStr("</i>");   break;
    case Texo::strike_begin:    exporter.PutStr("<del>");  break;
    case Texo::strike_end:      exporter.PutStr("</del>"); break;
    case Texo::underline_begin: exporter.PutStr("<ins>");  break;
    case Texo::underline_end:   exporter.PutStr("</ins>"); break;
    case Texo::link_begin:      BeginLink(piece);          break;
    case Texo::link_end:        exporter.PutStr("</a>");   break;
    case Texo::image:           Image(piece);              break;
    default:                    break;
    }
}

void TexoHTMLProducer::BeginLink(const Texo &piece)
{
    exporter.PutStr("<a");
    if (piece.link_url) {
        exporter.PutStr(" href='");
        exporter.PutStr(piece.link_url);
        exporter.Put('\'');
    }
    exporter.Put('>');
}

void TexoHTMLProducer::Image(const Texo &piece)
{
    exporter.PutStr("<img");
    if (piece.image_src) {
        exporter.PutStr(" src='");
        exporter.PutStr(piece.image_src);
        exporter.Put('\'');
    }
    if (piece.image_alt) {
        exporter.PutStr(" alt='");
        exporter.PutStr(piece.image_alt);
        exporter.Put('\'');
    }
    exporter.PutStr("/>");
}


TexoHTMLImporter::TexoHTMLImporter(TexoProducer &producer):
    TexoImporter(producer)
{}

void TexoHTMLImporter::Put(const char c)
{  // TODO by automata
    producer.Put(Texo(c));
}
