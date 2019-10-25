#include "plain.hpp"


TexoProducerPlain::TexoProducerPlain(TexoExporter &exporter):
    TexoProducer(exporter), quoted(false), newline(false)
{}

void TexoProducerPlain::Put(const Texo &piece)
{
    if (piece.c == '\n') {
        if (quoted) {
            exporter.Put("\n>");
        } else if (newline) {
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

void TexoProducerPlain::Put(const TexoParagraph &piece)
{
    if (piece.closing) {
        exporter.Put("\n\n");
        newline = true;
    }
}

void TexoProducerPlain::Put(const TexoQuote &piece)
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

void TexoProducerPlain::Put(const TexoLineBreak &piece)
{
    exporter.Put("\n\n");
    newline = true;
}

void TexoProducerPlain::Put(const TexoHorizontalRule &piece)
{
    if (!newline) {
        exporter.Put('\n');
    }
    exporter.Put("--------------------------------------------------\n");
    newline = false;
}


TexoImporterPlain::TexoImporterPlain(TexoProducer &producer):
    TexoImporter(producer), state(text)
{
    producer.Put(TexoParagraph());
}

TexoImporterPlain::~TexoImporterPlain()
{
    switch (state) {
    case rule:
        producer.Put('\n');
        for (;dash_count > 0; --dash_count) {
            producer.Put('-');
        }
    case text: case newline:
        producer.Put(TexoParagraph(true));
        break;
    case quote: case quote_newline:
        producer.Put(TexoQuote(true));
        break;
    case paragraph:
        break;
    }
}

void TexoImporterPlain::Put(char c)
{
    switch (state) {
    case text:          Text(c);         break;
    case newline:       Newline(c);      break;
    case paragraph:     Paragraph(c);    break;
    case quote:         Quote(c);        break;
    case quote_newline: QuoteNewline(c); break;
    case rule:          Rule(c);         break;
    }
}

void TexoImporterPlain::Put(const ScriptVariable &str)
{
    TexoImporter::Put(str);
}

void TexoImporterPlain::Put(FILE *file)
{
    TexoImporter::Put(file);
}

void TexoImporterPlain::Text(char c)
{
    switch (c) {
    case '\n': state = newline; break;
    default:   producer.Put(c); break;
    }
}

void TexoImporterPlain::Newline(char c)
{
    if (c == '\n') {
        state = paragraph;
    } else if (c == '-') {
        dash_count = 1;
        state = rule;
    } else {
        producer.Put('\n');
        state = text;
        Text(c);
    }
}

void TexoImporterPlain::Paragraph(char c)
{
    if (c == '>') {
        producer.Put(TexoParagraph(true));
        producer.Put(TexoQuote());
        state = quote;
    } else if (c == '-') {
        dash_count = 1;
        state = rule;
    } else if (c != '\n') {
        producer.Put(TexoParagraph(true));
        producer.Put(TexoParagraph());
        state = text;
        Text(c);
    }
}

void TexoImporterPlain::Quote(char c)
{
    switch (c) {
    case '\n': state = quote_newline; break;
    default:   producer.Put(c);       break;
    }
}

void TexoImporterPlain::QuoteNewline(char c)
{
    if (c == '>') {
        producer.Put('\n');
        state = quote;
    } else if (c == '\n') {
        producer.Put(TexoQuote(true));
        producer.Put(TexoParagraph());
        state = paragraph;
    } else if (c == '-') {
        producer.Put(TexoQuote(true));
        producer.Put(TexoParagraph());
        dash_count = 1;
        state = rule;
    } else {
        producer.Put(TexoQuote(true));
        producer.Put(TexoParagraph());
        state = text;
        Text(c);
    }
}

void TexoImporterPlain::Rule(char c)
{
    if (c == '\n') {
        producer.Put(TexoHorizontalRule());
        state = text;
    } else if (c == '-') {
        ++dash_count;
    } else {
        producer.Put('\n');
        for (;dash_count > 0; --dash_count) {
            producer.Put('-');
        }
        state = text;
        Text(c);
    }
}
