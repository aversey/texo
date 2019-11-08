#include "lines.hpp"


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Texo Lines Producer
 */
TexoProducerLines::TexoProducerLines(TexoExporter &exporter):
    TexoProducer(exporter), newline(true)
{}


bool TexoProducerLines::End()
{
    if (!newline) {
        exporter.Put('\n');
        newline = true;
    }
    return true;
}


bool TexoProducerLines::Put(char c)
{
    if (c == '\n') {
        if (!newline) {
            exporter.Put(' ');
            newline = true;
        }
    } else {
        exporter.Put(c);
        newline = false;
    }
    return true;
}


bool TexoProducerLines::Paragraph()
{
    if (!newline) {
        exporter.Put('\n');
        newline = true;
    }
    return true;
}

bool TexoProducerLines::Quote()
{
    if (!newline) {
        exporter.Put('\n');
    }
    exporter.Put("> ");
    newline = false;
    return true;
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Texo Lines Importer
 */
TexoImporterLines::TexoImporterLines(TexoProducer &producer):
    TexoImporter(producer), newline(true), quote(false)
{}


bool TexoImporterLines::TruePut(char c)
{
    if (c == '\n') {
        if (!newline) {
            newline = true;
            quote   = false;
        }
        return true;
    } else if (newline) {
        newline = false;
        if (c == '>') {
            quote = true;
            return producer.Quote();
        } else {
            return producer.Paragraph() && producer.Put(c);
        }
    } else if (quote) {
        if (c != ' ') {
            quote = false;
            return producer.Put(c);
        } else {
            return true;
        }
    } else {
        return producer.Put(c);
    }
}
