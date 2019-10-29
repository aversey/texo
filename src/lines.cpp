#include "lines.hpp"


TexoProducerLines::TexoProducerLines(TexoExporter &exporter):
    TexoProducer(exporter), newline(false)
{}

void TexoProducerLines::Put(const Texo &piece)
{
    if (piece.c == '\n') {
        if (!newline) {
            exporter.Put(' ');
        }
    } else {
        exporter.Put(piece.c);
        newline = false;
    }
}

void TexoProducerLines::Put(const TexoParagraph &piece)
{
    if (piece.closing) {
        exporter.Put('\n');
        newline = true;
    }
}

void TexoProducerLines::Put(const TexoQuote &piece)
{
    if (piece.closing) {
        exporter.Put('\n');
        newline = true;
    } else {
        exporter.Put('>');
    }
}

void TexoProducerLines::Put(const TexoLineBreak &piece)
{
    exporter.Put('\n');
    newline = true;
}


TexoImporterLines::TexoImporterLines(TexoProducer &producer):
    TexoImporter(producer), quoted(false), newline(true)
{}

TexoImporterLines::~TexoImporterLines()
{
    if (!newline) {
        if (quoted) {
            producer.Put(TexoQuote(true));
        } else {
            producer.Put(TexoParagraph(true));
        }
    }
}

void TexoImporterLines::Put(char c)
{
    if (c == '\n') {
        if (!newline) {
            if (quoted) {
                producer.Put(TexoQuote(true));
                quoted = false;
            } else {
                producer.Put(TexoParagraph(true));
            }
            newline = true;
        }
    } else if (newline && c == '>') {
        producer.Put(TexoQuote());
        newline = false;
        quoted  = true;
    } else {
        if (newline) {
            producer.Put(TexoParagraph());
            newline = false;
        }
        producer.Put(c);
    }
}

void TexoImporterLines::Put(const ScriptVariable &str)
{
    TexoImporter::Put(str);
}

void TexoImporterLines::Put(FILE *file)
{
    TexoImporter::Put(file);
}
