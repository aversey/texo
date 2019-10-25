#include "markdown.hpp"


TexoProducerMarkdown::TexoProducerMarkdown(TexoExporter &exporter):
    TexoProducer(exporter), quoted(false), newline(false), code(false)
{}

void TexoProducerMarkdown::Put(const Texo &piece)
{
    if (piece.c == '\n') {
        if (code) {
            exporter.Put('\n');
        } else if (quoted) {
            exporter.Put("\n>");
        } else if (header || newline) {
            exporter.Put(' ');
        } else {
            exporter.Put('\n');
            newline = true;
        }
    } else {
        exporter.Put(piece.c);
        newline = false;
    }
}

void TexoProducerMarkdown::Put(const TexoHeader &piece)
{
    if (piece.closing) {
        exporter.Put('\n');
        header  = false;
        newline = true;
    } else {
        if (!newline) {
            exporter.Put('\n');
        }
        if (piece.level <= 1) {
            exporter.Put("###### ");
        } else if (piece.level == 2) {
            exporter.Put("##### ");
        } else if (piece.level == 3) {
            exporter.Put("#### ");
        } else if (piece.level == 4) {
            exporter.Put("### ");
        } else if (piece.level == 5) {
            exporter.Put("## ");
        } else {
            exporter.Put("# ");
        }
        header = true;
    }
}

void TexoProducerMarkdown::Put(const TexoParagraph &piece)
{
    if (piece.closing) {
        exporter.Put("\n\n");
        newline = true;
    }
}

void TexoProducerMarkdown::Put(const TexoCode &piece)
{
    exporter.Put("\n```\n");
    code = !code;
}

void TexoProducerMarkdown::Put(const TexoQuote &piece)
{
    if (piece.closing) {
        exporter.Put("\n\n");
        quoted  = false;
        newline = true;
    } else {
        if (!newline) {
            exporter.Put('\n');
        }
        exporter.Put(">");
        quoted = true;
    }
}

void TexoProducerMarkdown::Put(const TexoMono &piece)
{
    exporter.Put("`");
}

void TexoProducerMarkdown::Put(const TexoBold &piece)
{
    exporter.Put("**");
}

void TexoProducerMarkdown::Put(const TexoItalic &piece)
{
    exporter.Put("*");
}

void TexoProducerMarkdown::Put(const TexoUnderline &piece)
{
    exporter.Put("++");
}

void TexoProducerMarkdown::Put(const TexoStrike &piece)
{
    exporter.Put("~~");
}

void TexoProducerMarkdown::Put(const TexoImage &piece)
{
    if (piece.path != "") {
        const bool link = piece.link != "";
        if (link) {
            exporter.Put('[');
        }
        exporter.Put("![");
        exporter.Put(piece.alt);
        exporter.Put("](");
        exporter.Put(piece.path);
        if (piece.title != "") {
            exporter.Put(" \"");
            exporter.Put(piece.title);
            exporter.Put('"');
        }
        exporter.Put(')');
        if (link) {
            exporter.Put("](");
            exporter.Put(piece.link);
            if (piece.title != "") {
                exporter.Put(" \"");
                exporter.Put(piece.title);
                exporter.Put('"');
            }
            exporter.Put(')');
        }
    }
}

void TexoProducerMarkdown::Put(const TexoLink &piece)
{
    if (piece.text != "") {
        exporter.Put('[');
        exporter.Put(piece.text);
        exporter.Put("](");
        exporter.Put(piece.link);
        if (piece.title != "") {
            exporter.Put(" \"");
            exporter.Put(piece.title);
            exporter.Put('"');
        }
        exporter.Put(')');
    }
}

void TexoProducerMarkdown::Put(const TexoLineBreak &piece)
{
    exporter.Put("\\\n");
    newline = true;
}

void TexoProducerMarkdown::Put(const TexoHorizontalRule &piece)
{
    if (!newline) {
        exporter.Put('\n');
    }
    exporter.Put("---\n");
    newline = false;
}
