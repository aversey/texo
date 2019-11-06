#include "lines.hpp"


TexoProducerLines::TexoProducerLines(TexoExporter &exporter):
    TexoProducer(exporter), newline(true)
{}

void TexoProducerLines::End()
{
    if (!newline) {
        exporter.Put('\n');
        newline = true;
    }
}

void TexoProducerLines::Put(const Texo &piece)
{
    if (piece.c == '\n') {
        if (!newline) {
            exporter.Put(' ');
            newline = true;
        }
    } else {
        exporter.Put(piece.c);
        newline = false;
    }
}

void TexoProducerLines::Put(const TexoParagraph &piece)
{
    if (!newline) {
        exporter.Put('\n');
        newline = true;
    }
}

void TexoProducerLines::Put(const TexoQuote &piece)
{
    if (!newline) {
        exporter.Put('\n');
    }
    exporter.Put("> ");
    newline = false;
}


TexoImporterLines::TexoImporterLines(TexoProducer &producer):
    TexoImporter(producer), newline(true), quote(false)
{}

void TexoImporterLines::Put(char c)
{
    if (c == '\n') {
        if (!newline) {
            newline = true;
            quote   = false;
        }
    } else if (newline) {
        if (c == '>') {
            producer.Put(TexoQuote());
            quote = true;
        } else {
            producer.Put(TexoParagraph());
            producer.Put(c);
        }
        newline = false;
    } else if (quote) {
        if (c != ' ') {
            quote = false;
            producer.Put(c);
        }
    } else {
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
