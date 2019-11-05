#include "lines.hpp"


TexoProducerLines::TexoProducerLines(TexoExporter &exporter):
    TexoProducer(exporter), newline(false)
{}

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
    exporter.Put('\n');
    newline = true;
}

void TexoProducerLines::Put(const TexoQuote &piece)
{
    exporter.Put("\n>");
    newline = false;
}


TexoImporterLines::TexoImporterLines(TexoProducer &producer):
    TexoImporter(producer), newline(true)
{}

void TexoImporterLines::Put(char c)
{
    if (c == '\n') {
        if (!newline) {
            newline = true;
        }
    } else if (newline) {
        if (c == '>') {
            producer.Put(TexoQuote());
        } else {
            producer.Put(TexoParagraph());
            producer.Put(c);
        }
        newline = false;
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
