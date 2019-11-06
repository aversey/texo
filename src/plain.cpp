#include "plain.hpp"


TexoProducerPlain::TexoProducerPlain(TexoExporter &exporter):
    TexoProducer(exporter), quoted(false), newline(false), nospace(true)
{}

void TexoProducerPlain::End()
{
    if (!newline) {
        exporter.Put('\n');
        newline = true;
    }
}

void TexoProducerPlain::Put(const Texo &piece)
{
    if (piece.c == '\n') {
        if (quoted) {
            exporter.Put("\n> ");
        } else if (!nospace) {
            if (newline) {
                exporter.Put(' ');
            } else {
                exporter.Put('\n');
                newline = true;
            }
        }
    } else {
        exporter.Put(piece.c);
        newline = false;
        nospace = false;
    }
}

void TexoProducerPlain::Put(const TexoParagraph &piece)
{
    if (!nospace) {
        exporter.Put("\n\n");
    }
    quoted  = false;
    nospace = true;
}

void TexoProducerPlain::Put(const TexoQuote &piece)
{
    if (!nospace) {
        exporter.Put("\n\n");
    }
    exporter.Put("> ");
    quoted = true;
}

void TexoProducerPlain::Put(const TexoHorizontalRule &piece)
{
    if (!newline && !nospace) {
        exporter.Put('\n');
    }
    exporter.Put("--------------------------------------------------\n");
    nospace = true;
}


TexoImporterPlain::TexoImporterPlain(TexoProducer &producer):
    TexoImporter(producer), state(text)
{
    producer.Put(TexoParagraph());
}

void TexoImporterPlain::End()
{
    if (state == rule) {
        producer.Put(TexoHorizontalRule());
        state = text;
    }
    producer.End();
}

void TexoImporterPlain::Put(char c)
{
    switch (state) {
    case text:           Text(c);          break;
    case newline:        Newline(c);       break;
    case paragraph:      Paragraph(c);     break;
    case quote_pre:      QuotePre(c);      break;
    case quote:          Quote(c);         break;
    case quote_newline:  QuoteNewline(c);  break;
    case rule:           Rule(c);          break;
    case paragraph_rule: ParagraphRule(c); break;
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
        producer.Put(TexoQuote());
        state = quote_pre;
    } else if (c == '-') {
        dash_count = 1;
        state = paragraph_rule;
    } else if (c != '\n') {
        producer.Put(TexoParagraph());
        state = text;
        Text(c);
    }
}

void TexoImporterPlain::QuotePre(char c)
{
    if (c != ' ') {
        state = quote;
        Quote(c);
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
        state = quote_pre;
    } else if (c == '\n') {
        state = paragraph;
    } else if (c == '-') {
        dash_count = 1;
        state = paragraph_rule;
    } else {
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

void TexoImporterPlain::ParagraphRule(char c)
{
    if (c == '\n') {
        producer.Put(TexoParagraph());
    }
    Rule(c);
}
