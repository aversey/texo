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
    Close();
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

void TexoProducerMarkdown::Put(const TexoParagraph &piece)
{
    Close();
    exporter.Put('\n');
}

void TexoProducerMarkdown::Put(const TexoCode &piece)
{
    Close();
    exporter.Put("```\n");
    code = true;
}

void TexoProducerMarkdown::Put(const TexoQuote &piece)
{
    Close();
    exporter.Put('>');
    quoted = true;
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

void TexoProducerMarkdown::Put(const TexoHorizontalRule &piece)
{
    if (!newline) {
        exporter.Put('\n');
    }
    exporter.Put("---\n");
    newline = true;
}

void TexoProducerMarkdown::Close()
{
    if (code) {
        exporter.Put("\n```");
        code = false;
    }
    exporter.Put('\n');
    header  = false;
    quoted  = false;
    newline = true;
}


TexoImporterMarkdown::TexoImporterMarkdown(TexoProducer &producer):
    TexoImporter(producer), state(start)
{}

void TexoImporterMarkdown::Put(char c)
{
    switch (state) {
    case error:                          break;
    case start:         Start(c);        break;
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
    case rule:          Rule(c);         break;
    case paragraph:     Paragraph(c);    break;
    case header:        Header(c);       break;
    case code:          Code(c);         break;
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

void TexoImporterMarkdown::Start(char c)
{
    if (c == '\n') {
        state = paragraph;
    } else if (c == '#') {
        state        = header;
        header_level = 6;
    } else if (c == '>') {
        producer.Put(TexoQuote());
    } else if (c == '-') {
        producer.Put(TexoParagraph());
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

void TexoImporterMarkdown::Text(char c)
{
    back = text;
    switch (c) {
    case '\\': state = backslash; break;
    case '\n': state = newline;   break;
    case '*':  state = asterisk;  break;
    case '_':  state = underline; break;
    case '+':  state = plus;      break;
    case '~':  state = tilde;     break;
    case '`':  Backquote();       break;
    default:   producer.Put(c);
    }
}

void TexoImporterMarkdown::HeaderText(char c)
{
    back = header_text;
    switch (c) {
    case '\\': state = backslash; break;
    case '\n': state = paragraph; break;
    case '*':  state = asterisk;  break;
    case '_':  state = underline; break;
    case '+':  state = plus;      break;
    case '~':  state = tilde;     break;
    case '`':  Backquote();       break;
    default:   producer.Put(c);
    }
}

void TexoImporterMarkdown::QuoteText(char c)
{
    back = quote_text;
    switch (c) {
    case '\\': state = backslash;     break;
    case '\n': state = quote_newline; break;
    case '*':  state = asterisk;      break;
    case '_':  state = underline;     break;
    case '+':  state = plus;          break;
    case '~':  state = tilde;         break;
    case '`':  Backquote();           break;
    default:   producer.Put(c);
    }
}

void TexoImporterMarkdown::QuoteNewline(char c)
{
    if (c == '>') {
        producer.Put('\n');
        state = quote_text;
    } else {
        state = paragraph;
        Put(c);
    }
}

void TexoImporterMarkdown::CodeText(char c)
{
    back = code_text;
    switch (c) {
    case '\\': state = backslash;    break;
    case '\n': state = code_newline; break;
    case '*':  state = asterisk;     break;
    case '_':  state = underline;    break;
    case '+':  state = plus;         break;
    case '~':  state = tilde;        break;
    case '`':  state = error;        break;
    default:   producer.Put(c);
    }
}

void TexoImporterMarkdown::CodeNewline(char c)
{
    if (c == '`') {
        state            = code_end;
        code_quote_count = 1;
    } else {
        producer.Put('\n');
        state = code_text;
        Put(c);
    }
}

void TexoImporterMarkdown::CodeEnd(char c)
{
    if (c == '`') {
        ++code_quote_count;
    } else  if (c == '\n') {
        if (code_quote_count == 3) {
            state = paragraph;
        } else {
            state = error;
        }
    } else {
        state = error;
    }
}

void TexoImporterMarkdown::Backslash(char c)
{
    producer.Put(c);
    state = back;
}

void TexoImporterMarkdown::Asterisk(char c)
{
    state = back;
    if (c == '*') {
        producer.Put(TexoBold());
    } else {
        producer.Put(TexoItalic());
        Put(c);
    }
}

void TexoImporterMarkdown::Underline(char c)
{
    state = back;
    if (c == '_') {
        producer.Put(TexoBold());
    } else {
        producer.Put(TexoItalic());
        Put(c);
    }
}

void TexoImporterMarkdown::Plus(char c)
{
    state = back;
    if (c == '+') {
        producer.Put(TexoUnderline());
    } else {
        producer.Put('+');
        Put(c);
    }
}

void TexoImporterMarkdown::Tilde(char c)
{
    state = back;
    if (c == '~') {
        producer.Put(TexoStrike());
    } else {
        producer.Put('~');
        Put(c);
    }
}

void TexoImporterMarkdown::Newline(char c)
{
    if (c == '\n') {
        state = paragraph;
    } else if (c == '#') {
        state        = header;
        header_level = 6;
    } else if (c == '>') {
        producer.Put(TexoQuote());
        state = quote_text;
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
    } else if (c == '\n' && rule_dash_count >= 3) {
        producer.Put(TexoHorizontalRule());
        state = newline;
    } else {
        producer.Put(' ');
        for (int i = 0; i < rule_dash_count; ++i) {
            producer.Put('-');
        }
        state = text;
        Put(c);
    }
}

void TexoImporterMarkdown::Paragraph(char c)
{
    if (c == '#') {
        state        = header;
        header_level = 6;
    } else if (c == '>') {
        producer.Put(TexoQuote());
        state = quote_text;
    } else if (c == '-') {
        producer.Put(TexoParagraph());
        state           = rule;
        rule_dash_count = 1;
    } else if (c == '`') {
        state            = code;
        code_quote_count = 1;
    } else if (c != '\n') {
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
        } else {
            state = error;
        }
    } else {
        producer.Put(TexoHeader(header_level));
        state = header_text;
        Put(c);
    }
}

void TexoImporterMarkdown::Code(char c)
{
    if (c == '`') {
        ++code_quote_count;
    } else  if (c == '\n') {
        if (code_quote_count == 3) {
            producer.Put(TexoCode());
            state = code_text;
        } else {
            state = error;
        }
    } else {
        state = error;
    }
}

void TexoImporterMarkdown::Backquote()
{
    producer.Put(TexoMono());
}
