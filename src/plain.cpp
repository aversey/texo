#include "plain.hpp"


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Texo Plain Producer
 */
TexoProducerPlain::TexoProducerPlain(TexoExporter & exporter):
    TexoProducer(exporter), quoted(false), newline(false), nospace(true)
{}


bool TexoProducerPlain::End()
{
    if (!newline) {
        exporter.Put('\n');
        newline = true;
    }
    return true;
}


bool TexoProducerPlain::Put(char c)
{
    if (c == '\n') {
        if (quoted) {
            return exporter.Put("\n> ");
        } else if (!nospace) {
            if (newline) {
                return exporter.Put(' ');
            } else {
                newline = true;
                return exporter.Put('\n');
            }
        } else {
            return true;
        }
    } else {
        newline = false;
        nospace = false;
        return exporter.Put(c);
    }
}


bool TexoProducerPlain::Paragraph()
{
    quoted = false;
    if (!nospace) {
        nospace = true;
        return exporter.Put("\n\n");
    } else {
        return true;
    }
}

bool TexoProducerPlain::Quote()
{
    quoted = true;
    if (!nospace) {
        return exporter.Put("\n\n> ");
    } else {
        return exporter.Put("> ");
    }
}

bool TexoProducerPlain::PutHorizontalRule()
{
    bool ok = true;
    if (!newline && !nospace) {
        ok = exporter.Put('\n');
    }
    nospace = true;
    return ok
        && exporter.Put(
            "--------------------------------------------------\n");
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Texo Plain Importer
 */
TexoImporterPlain::TexoImporterPlain(TexoProducer & producer):
    TexoImporter(producer), state(text)
{}


bool TexoImporterPlain::TrueEnd()
{
    if (state == rule) {
        ok    = producer.PutHorizontalRule();
        state = text;
    }
    return ok && producer.End();
}


bool TexoImporterPlain::TruePut(char c)
{
    switch (state) {
    case text:
        Text(c);
        break;
    case newline:
        Newline(c);
        break;
    case paragraph:
        Paragraph(c);
        break;
    case quote_pre:
        QuotePre(c);
        break;
    case quote:
        Quote(c);
        break;
    case quote_newline:
        QuoteNewline(c);
        break;
    case rule:
        Rule(c);
        break;
    case paragraph_rule:
        ParagraphRule(c);
        break;
    }
    return ok;
}


void TexoImporterPlain::Text(char c)
{
    switch (c) {
    case '\n':
        state = newline;
        break;
    default:
        ok = producer.Put(c);
        break;
    }
}

void TexoImporterPlain::Newline(char c)
{
    if (c == '\n') {
        state = paragraph;
    } else if (c == '-') {
        dash_count = 1;
        state      = rule;
    } else {
        state = text;
        ok    = producer.Put('\n');
        if (ok) {
            Text(c);
        }
    }
}

void TexoImporterPlain::Paragraph(char c)
{
    if (c == '>') {
        ok    = producer.Quote();
        state = quote_pre;
    } else if (c == '-') {
        dash_count = 1;
        state      = paragraph_rule;
    } else if (c != '\n') {
        state = text;
        ok    = producer.Paragraph();
        if (ok) {
            Text(c);
        }
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
    case '\n':
        state = quote_newline;
        break;
    default:
        ok = producer.Put(c);
        break;
    }
}

void TexoImporterPlain::QuoteNewline(char c)
{
    if (c == '>') {
        ok    = producer.Put('\n');
        state = quote_pre;
    } else if (c == '\n') {
        state = paragraph;
    } else if (c == '-') {
        dash_count = 1;
        state      = paragraph_rule;
    } else {
        state = text;
        ok    = producer.Paragraph();
        if (ok) {
            Text(c);
        }
    }
}

void TexoImporterPlain::Rule(char c)
{
    if (c == '\n') {
        ok    = producer.PutHorizontalRule();
        state = text;
    } else if (c == '-') {
        ++dash_count;
    } else {
        ok = producer.Put('\n');
        for (; ok && dash_count > 0; --dash_count) {
            ok = producer.Put('-');
        }
        state = text;
        if (ok) {
            Text(c);
        }
    }
}

void TexoImporterPlain::ParagraphRule(char c)
{
    if (c == '\n') {
        ok = producer.Paragraph();
    }
    if (ok) {
        Rule(c);
    }
}
