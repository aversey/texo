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
        exporter.Put("\n<h6>\n");
        opened_block = header_6;
    } else if (piece.level == 2) {
        exporter.Put("\n<h5>\n");
        opened_block = header_5;
    } else if (piece.level == 3) {
        exporter.Put("\n<h4>\n");
        opened_block = header_4;
    } else if (piece.level == 4) {
        exporter.Put("\n<h3>\n");
        opened_block = header_3;
    } else if (piece.level == 5) {
        exporter.Put("\n<h2>\n");
        opened_block = header_2;
    } else {
        exporter.Put("\n<h1>\n");
        opened_block = header_1;
    }
}

void TexoProducerHTML::Put(const TexoParagraph &piece)
{
    Close();
    opened_block = paragraph;
    exporter.Put("\n<p>\n");
}

void TexoProducerHTML::Put(const TexoCode &piece)
{
    Close();
    opened_block = code;
    exporter.Put("\n<pre>\n");
}

void TexoProducerHTML::Put(const TexoQuote &piece)
{
    Close();
    opened_block = quote;
    exporter.Put("\n<blockquote><p>\n");
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
    exporter.Put("\n<hr/>\n");
}

void TexoProducerHTML::Close()
{
    switch (opened_block) {
    case none:                                         break;
    case header_1:  exporter.Put("\n</h1>\n");             break;
    case header_2:  exporter.Put("\n</h2>\n");             break;
    case header_3:  exporter.Put("\n</h3>\n");             break;
    case header_4:  exporter.Put("\n</h4>\n");             break;
    case header_5:  exporter.Put("\n</h5>\n");             break;
    case header_6:  exporter.Put("\n</h6>\n");             break;
    case paragraph: exporter.Put("\n</p>\n");              break;
    case quote:     exporter.Put("\n</p></blockquote>\n"); break;
    case code:      exporter.Put("\n</pre>\n");            break;
    }
}
