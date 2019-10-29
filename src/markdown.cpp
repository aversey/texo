#include "markdown.hpp"


TexoProducerMarkdown::TexoProducerMarkdown(TexoExporter &exporter):
    TexoProducer(exporter),
    quoted(false), newline(false), header(false), code(false)
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


TexoImporterMarkdown::TexoImporterMarkdown(TexoProducer &producer):
    TexoImporter(producer), state(text)
{}

TexoImporterMarkdown::~TexoImporterMarkdown()
{}

void TexoImporterMarkdown::Put(char c)
{
    switch (state) {
    case text:          Text(c);         break;
    case header_text:   HeaderText(c);   break;
    case quote_text:    QuoteText(c);    break;
    case quote_newline: QuoteNewline(c); break;
    case code_text:     CodeText(c);     break;
    case code_newline:  CodeNewline(c);  break;
    case code_end:      CodeEnd(c);      break;
    case backslash:     Backslash(c);    break;
    case asterisk:      Asterisk(c);     break;
    case underline:     Underline(c);    break;
    case plus:          Plus(c);         break;
    case tilde:         Tilde(c);        break;
    case newline:       Newline(c);      break;
    case paragraph:     Paragraph(c);    break;
    case header:        Header(c);       break;
    case code:          Code(c);         break;
    case quote:         Quote(c);        break;
    }
}

void TexoImporterMarkdown::Put(const ScriptVariable &str)
{
    TexoImporter::Put(str);
}

void TexoImporterMarkdown::Put(FILE *file)
{
    TexoImporter::Put(file);
}

void TexoImporterMarkdown::Text(char c)
{
    wrapping_state = text;
    switch (c) {
    case '\\': state = backslash; break;
    case '\n': state = newline;   break;
    case '*':  state = asterisk;  break;
    case '_':  state = underline; break;
    case '~':  state = tilde;     break;
    case '`':  Backquote();       break;
    default:   producer.Put(c);
    }
}

void TexoImporterMarkdown::HeaderText(char c)
{
    wrapping_state = header_text;
    switch (c) {
    case '\\': state = backslash; break;
    case '\n': EndHeader();       break;
    case '*':  state = asterisk;  break;
    case '_':  state = underline; break;
    case '~':  state = tilde;     break;
    case '`':  Backquote();       break;
    default:   producer.Put(c);
    }
}

void TexoImporterMarkdown::QuoteText(char c)
{
    wrapping_state = header_text;
    switch (c) {
    case '\\': state = backslash;     break;
    case '\n': state = quote_newline; break;
    case '*':  state = asterisk;      break;
    case '_':  state = underline;     break;
    case '~':  state = tilde;         break;
    case '`':  Backquote();           break;
    default:   producer.Put(c);
    }
}

void TexoImporterMarkdown::QuoteNewline(char c);
void TexoImporterMarkdown::CodeText(char c);
void TexoImporterMarkdown::CodeNewline(char c);
void TexoImporterMarkdown::CodeEnd(char c);

void TexoImporterMarkdown::Backslash(char c)
{
    producer.Put(c);
    state = wrapping_state;
}

void TexoImporterMarkdown::Asterisk(char c)
{
    state = wrapping_state;
    if (c == '*') {
        producer.Put(TexoBold(is_bold));
        is_bold = !is_bold;
    } else {
        producer.Put(TexoItalic(is_italic));
        is_italic = !is_italic;
        Put(c);
    }
}

void TexoImporterMarkdown::Underline(char c)
{
    state = wrapping_state;
    if (c == '_') {
        producer.Put(TexoBold(is_bold));
        is_bold = !is_bold;
    } else {
        producer.Put(TexoItalic(is_italic));
        is_italic = !is_italic;
        Put(c);
    }
}

void TexoImporterMarkdown::Plus(char c)
{
    state = wrapping_state;
    if (c == '+') {
        producer.Put(TexoUnderline(is_underline));
        is_underline = !is_underline;
    } else {
        producer.Put('+');
        Put(c);
    }
}

void TexoImporterMarkdown::Tilde(char c)
{
    state = wrapping_state;
    if (c == '~') {
        producer.Put(TexoStrike(is_strike));
        is_strike = !is_strike;
    } else {
        producer.Put('~');
        Put(c);
    }
}

void TexoImporterMarkdown::Backquote()
{
    producer.Put(TexoMono(is_mono));
    is_mono = !is_mono;
}

void TexoImporterMarkdown::Newline(char c)
{
    if (c == '\n') {
        state = paragraph;
    } else if (c == '#') {
        state        = header;
        header_level = 6;
    } else if (c == '>') {
        state = quote;
    } else if (c == '-') {
        state           = rule;
        rule_dash_count = 1;
    } else if (c == '`') {
        state            = code;
        code_quote_count = 1;
    } else {
        producer.Put(' ');
        state = text;
        Put(c);
    }
}

void TexoImporterMarkdown::Rule(char c)
{
    if (c == '-') {
        ++rule_dash_count;
    } else {
        if (rule_dash_count >= 3) {
            producer.Put(TexoHorizontalRule());
        } else {
            if (wrapping_state == paragraph) {
                producer.Put(TexoParagraph());
            }
            for (int i = 0; i < rule_dash_count; ++i) {
                producer.Put('-');
            }
        }
    }
}

void TexoImporterMarkdown::Paragraph(char c)
{
    wrapping_state = paragraph;
    if (c == '#') {
        state        = header;
        header_level = 6;
    } else if (c == '>') {
        state = quote;
    } else if (c == '-') {
        state           = rule;
        rule_dash_count = 1;
    } else if (c == '`') {
        state            = code;
        code_quote_count = 1;
    } else {
        producer.Put(TexoParagraph());
        state = text;
        Put(c);
    }
}

void TexoImporterMarkdown::Header(char c)
{
    if (c == '#') {
        if (header_level > 1) {
            --header_level;
        }
    } else {
        producer.Put(TexoHeader(header_level));
        state = header_text;
    }
}

void TexoImporterMarkdown::EndHeader()
{
    producer.Put(TexoHeader(header_level, true));
    state = paragraph;
}
