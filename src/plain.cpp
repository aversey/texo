#include "plain.hpp"


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Texo Plain Producer
 */
TexoProducerPlain::TexoProducerPlain(TexoExporter &exporter):
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
    return ok && exporter.Put("------------------------------------------\n");
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Texo Plain Importer
 */
TexoImporterPlain::TexoImporterPlain(TexoProducer &producer):
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
    case text:           return Text(c);
    case newline:        return Newline(c);
    case paragraph:      return Paragraph(c);
    case quote_pre:      return QuotePre(c);
    case quote:          return Quote(c);
    case quote_newline:  return QuoteNewline(c);
    case rule:           return Rule(c);
    case paragraph_rule: return ParagraphRule(c);
    }
    return ok;  // Impossible
}


bool TexoImporterPlain::Text(char c)
{
    if (c == '\n') {
        state = newline;
    } else {
        ok = producer.Put(c);
    }
    return ok;
}

bool TexoImporterPlain::Newline(char c)
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
    return ok;
}

bool TexoImporterPlain::Paragraph(char c)
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
    return ok;
}

bool TexoImporterPlain::QuotePre(char c)
{
    if (c != ' ') {
        state = quote;
        Quote(c);
    }
    return ok;
}

bool TexoImporterPlain::Quote(char c)
{
    if (c == '\n') {
        state = quote_newline;
    } else {
        ok = producer.Put(c);
    }
    return ok;
}

bool TexoImporterPlain::QuoteNewline(char c)
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
    return ok;
}

bool TexoImporterPlain::Rule(char c)
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
    return ok;
}

bool TexoImporterPlain::ParagraphRule(char c)
{
    if (c == '\n') {
        ok = producer.Paragraph();
    }
    return ok && Rule(c);
}
