#include "html.hpp"


TexoProducerHTML::TexoProducerHTML(TexoExporter &exporter):
    TexoProducer(exporter)
{}

void TexoProducerHTML::Put(const Texo &piece)
{
    switch (piece.c) {
    case '<': exporter.Put("&lt;");  break;
    case '>': exporter.Put("&gt;");  break;
    case '&': exporter.Put("&amp;"); break;
    default:  exporter.Put(piece.c); break;
    }
}

void TexoProducerHTML::Put(const TexoHeader &piece)
{
    exporter.Put('<');
    if (piece.closing) {
        exporter.Put('/');
    }
    if (piece.level <= 1) {
        exporter.Put("h6>");
    } else if (piece.level == 2) {
        exporter.Put("h5>");
    } else if (piece.level == 3) {
        exporter.Put("h4>");
    } else if (piece.level == 4) {
        exporter.Put("h3>");
    } else if (piece.level == 5) {
        exporter.Put("h2>");
    } else {
        exporter.Put("h1>");
    }
}

void TexoProducerHTML::Put(const TexoParagraph &piece)
{
    if (piece.closing) {
        exporter.Put("</p>");
    } else {
        exporter.Put("<p>");
    }
}

void TexoProducerHTML::Put(const TexoCode &piece)
{
    if (piece.closing) {
        exporter.Put("</pre>");
    } else {
        exporter.Put("<pre>");
    }
}

void TexoProducerHTML::Put(const TexoQuote &piece)
{
    if (piece.closing) {
        exporter.Put("</p></blockquote>");
    } else {
        exporter.Put("<blockquote><p>");
    }
}

void TexoProducerHTML::Put(const TexoMono &piece)
{
    if (piece.closing) {
        exporter.Put("</code>");
    } else {
        exporter.Put("<code>");
    }
}

void TexoProducerHTML::Put(const TexoBold &piece)
{
    if (piece.closing) {
        exporter.Put("</b>");
    } else {
        exporter.Put("<b>");
    }
}

void TexoProducerHTML::Put(const TexoItalic &piece)
{
    if (piece.closing) {
        exporter.Put("</i>");
    } else {
        exporter.Put("<i>");
    }
}

void TexoProducerHTML::Put(const TexoUnderline &piece)
{
    if (piece.closing) {
        exporter.Put("</ins>");
    } else {
        exporter.Put("<ins>");
    }
}

void TexoProducerHTML::Put(const TexoStrike &piece)
{
    if (piece.closing) {
        exporter.Put("</del>");
    } else {
        exporter.Put("<del>");
    }
}

void TexoProducerHTML::Put(const TexoImage &piece)
{
    if (piece.path != "") {
        bool link  = piece.link != "";
        bool title = piece.title != "";
        if (link) {
            exporter.Put("<a href='");
            exporter.Put(piece.link);
            if (title) {
                exporter.Put("' title='");
                exporter.Put(piece.title);
            }
            exporter.Put("'>");
        }
        exporter.Put("<img src='");
        exporter.Put(piece.path);
        if (piece.alt != "") {
            exporter.Put("' alt='");
            exporter.Put(piece.alt);
        }
        if (title) {
            exporter.Put("' title='");
            exporter.Put(piece.title);
        }
        exporter.Put("'/>");
        if (link) {
            exporter.Put("</a>");
        }
    }
}

void TexoProducerHTML::Put(const TexoLink &piece)
{
    if (piece.text != "" && piece.link != "") {
        exporter.Put("<a href='");
        exporter.Put(piece.link);
        if (piece.title != "") {
            exporter.Put("' title='");
            exporter.Put(piece.title);
        }
        exporter.Put("'>");
        exporter.Put(piece.text);
        exporter.Put("</a>");
    }
}

void TexoProducerHTML::Put(const TexoLineBreak &piece)
{
    exporter.Put("<br/>");
}

void TexoProducerHTML::Put(const TexoHorizontalRule &piece)
{
    exporter.Put("<hr/>");
}


TexoImporterHTML::TexoImporterHTML(TexoProducer &producer):
    TexoImporter(producer)
{}

void TexoImporterHTML::Put(char c)
{  // TODO
    producer.Put(Texo(c));
}

void TexoImporterHTML::Put(const ScriptVariable &str)
{
    TexoImporter::Put(str);
}

void TexoImporterHTML::Put(FILE *file)
{
    TexoImporter::Put(file);
}
