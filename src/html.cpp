#include "html.hpp"


TexoProducerHTML::TexoProducerHTML(TexoExporter &exporter):
    TexoProducer(exporter), opened_block(none)
{}

void TexoProducerHTML::End()
{
    Close();
    opened_block = none;
}

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
    Close();
    if (piece.level <= 1) {
        exporter.Put("<h6>");
        opened_block = header_6;
    } else if (piece.level == 2) {
        exporter.Put("<h5>");
        opened_block = header_5;
    } else if (piece.level == 3) {
        exporter.Put("<h4>");
        opened_block = header_4;
    } else if (piece.level == 4) {
        exporter.Put("<h3>");
        opened_block = header_3;
    } else if (piece.level == 5) {
        exporter.Put("<h2>");
        opened_block = header_2;
    } else {
        exporter.Put("<h1>");
        opened_block = header_1;
    }
}

void TexoProducerHTML::Put(const TexoParagraph &piece)
{
    Close();
    opened_block = paragraph;
    exporter.Put("<p>");
}

void TexoProducerHTML::Put(const TexoCode &piece)
{
    Close();
    opened_block = code;
    exporter.Put("<pre>");
}

void TexoProducerHTML::Put(const TexoQuote &piece)
{
    Close();
    opened_block = quote;
    exporter.Put("<blockquote><p>");
}

void TexoProducerHTML::Put(const TexoMono &piece)
{
    static bool closing = false;
    if (closing) {
        exporter.Put("</code>");
    } else {
        exporter.Put("<code>");
    }
    closing = !closing;
}

void TexoProducerHTML::Put(const TexoBold &piece)
{
    static bool closing = false;
    if (closing) {
        exporter.Put("</b>");
    } else {
        exporter.Put("<b>");
    }
    closing = !closing;
}

void TexoProducerHTML::Put(const TexoItalic &piece)
{
    static bool closing = false;
    if (closing) {
        exporter.Put("</i>");
    } else {
        exporter.Put("<i>");
    }
    closing = !closing;
}

void TexoProducerHTML::Put(const TexoUnderline &piece)
{
    static bool closing = false;
    if (closing) {
        exporter.Put("</ins>");
    } else {
        exporter.Put("<ins>");
    }
    closing = !closing;
}

void TexoProducerHTML::Put(const TexoStrike &piece)
{
    static bool closing = false;
    if (closing) {
        exporter.Put("</del>");
    } else {
        exporter.Put("<del>");
    }
    closing = !closing;
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

void TexoProducerHTML::Put(const TexoHorizontalRule &piece)
{
    exporter.Put("<hr/>");
}

void TexoProducerHTML::Close()
{
    switch (opened_block) {
    case none:                                         break;
    case header_1:  exporter.Put("</h1>");             break;
    case header_2:  exporter.Put("</h2>");             break;
    case header_3:  exporter.Put("</h3>");             break;
    case header_4:  exporter.Put("</h4>");             break;
    case header_5:  exporter.Put("</h5>");             break;
    case header_6:  exporter.Put("</h6>");             break;
    case paragraph: exporter.Put("</p>");              break;
    case quote:     exporter.Put("</p></blockquote>"); break;
    case code:      exporter.Put("</pre>");            break;
    }
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
