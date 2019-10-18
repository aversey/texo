#include "plain.hpp"


TexoProducerPlain::TexoProducerPlain(TexoExporter &exporter):
    TexoProducer(exporter)
{}

void TexoProducerPlain::Put(const Texo &piece)
{
    if (quoted && piece.c == '\n') {
        exporter.Put("\n> ");
    } else {
        exporter.Put(piece.c);
    }
}

void TexoProducerPlain::Put(const TexoHeader &piece)
{
    if (!piece.closing) {
        exporter.Put("\n\n# ");
    }
}

void TexoProducerPlain::Put(const TexoParagraph &piece)
{
    if (piece.closing) {
        exporter.Put("\n\n");
    }
}

void TexoProducerPlain::Put(const TexoQuote &piece)
{
    if (piece.closing) {
        quoted = false;
        exporter.Put("\n\n");
    } else {
        quoted = true;
        exporter.Put("\n\n> ");
    }
}

void TexoProducerPlain::Put(const TexoLineBreak &piece)
{
    exporter.Put(" \n");
}

void TexoProducerPlain::Put(const TexoHorizontalRule &piece)
{
    exporter.Put("\n--------------------------------------------------\n");
}


TexoImporterPlain::TexoImporterPlain(TexoProducer &producer):
    TexoImporter(producer), state(text)
{
    producer.Put(TexoParagraph());
}

TexoImporterPlain::~TexoImporterPlain()
{
    switch (state) {
    case text: case space: case newline:
        producer.Put(TexoParagraph(true));
        break;
    case quote: case quote_newline:
        producer.Put(TexoQuote(true));
        break;
    case header:
        producer.Put(TexoHeader(true));
        break;
    case paragraph:
        break;
    }
}

void TexoImporterPlain::Put(char c)
{
    switch (state) {
    case text:                Text(c);              break;
    case space:               Space(c);             break;
    case newline:             Newline(c);           break;
    case paragraph:           Paragraph(c);         break;
    case quote:               Quote(c);             break;
    case quote_newline:       QuoteNewline(c);      break;
    case header:              Header(c);            break;
    }
}

void TexoImporterPlain::Put(const ScriptVariable &str)
{
    TexoImporter::Put(str);
}

void TexoImporterPlain::Text(char c)
{
    switch (c) {
    case ' ':  state = space;         break;
    case '\n': state = newline;       break;
    default:   producer.Put(Texo(c)); break;
    }
}

void TexoImporterPlain::Space(char c)
{
    if (c == '\n') {
        producer.Put(TexoLineBreak());
        state = text;
    } else if (c == ' ') {
        producer.Put(Texo(' '));
    } else {
        producer.Put(Texo(' '));
        producer.Put(Texo(c));
        state = text;
    }
}

void TexoImporterPlain::Newline(char c)
{
    if (c == '\n') {
        producer.Put(TexoParagraph(true));
        state = paragraph;
    } else {
        producer.Put(Texo('\n'));
        producer.Put(Texo(c));
        state = text;
    }
}

void TexoImporterPlain::Paragraph(char c)
{
    if (c == '>') {
        producer.Put(TexoQuote());
        state = quote;
    } else if (c == '#') {
        producer.Put(TexoHeader(1));
        state = header;
    } else {
        producer.Put(TexoParagraph());
        producer.Put(Texo(c));
        state = text;
    }
}

void TexoImporterPlain::Quote(char c)
{
    switch (c) {
    case '\n': state = quote_newline; break;
    default:   producer.Put(Texo(c)); break;
    }
}

void TexoImporterPlain::QuoteNewline(char c)
{
    if (c == '>') {
        state = quote;
    } else if (c == '\n') {
        producer.Put(TexoQuote(true));
        state = paragraph;
    } else {
        producer.Put(TexoQuote(true));
        producer.Put(TexoParagraph());
        producer.Put(Texo(c));
        state = text;
    }
}

void TexoImporterPlain::Header(char c)
{
    if (c == '\n') {
        producer.Put(TexoHeader(1, true));
        state = paragraph;
    } else {
        producer.Put(Texo(c));
    }
}
