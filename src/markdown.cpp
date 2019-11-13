#include "markdown.hpp"


TexoProducerMarkdown::TexoProducerMarkdown(TexoExporter & exporter):
    TexoProducer(exporter), quoted(false), newline(false), header(false),
    code(false), nospace(true)
{}

bool TexoProducerMarkdown::End()
{
    if (!newline) {
        newline = true;
        return exporter.Put('\n');
    }
    return true;
}

bool TexoProducerMarkdown::Put(char c)
{
    bool ok = true;
    if (c == '\n') {
        if (code) {
            ok = exporter.Put('\n');
        } else if (quoted) {
            ok = exporter.Put("\n> ");
        } else if (!nospace) {
            if (header || newline) {
                ok = exporter.Put(' ');
            } else {
                ok      = exporter.Put('\n');
                newline = true;
            }
        }
    } else {
        if (c == '*' || c == '_' || c == '`' || c == '+' || c == '~') {
            ok = exporter.Put('\\');
        }
        ok      = ok && exporter.Put(c);
        newline = false;
        nospace = false;
    }
    return ok;
}


bool TexoProducerMarkdown::Code()
{
    bool ok = Close();
    code    = true;
    return ok && exporter.Put("```\n");
}

bool TexoProducerMarkdown::Header(int level)
{
    bool ok = Close();
    if (level <= 1) {
        ok = ok && exporter.Put("###### ");
    } else if (level == 2) {
        ok = ok && exporter.Put("##### ");
    } else if (level == 3) {
        ok = ok && exporter.Put("#### ");
    } else if (level == 4) {
        ok = ok && exporter.Put("### ");
    } else if (level == 5) {
        ok = ok && exporter.Put("## ");
    } else {
        ok = ok && exporter.Put("# ");
    }
    header = true;
    return ok;
}

bool TexoProducerMarkdown::Paragraph()
{
    return Close();
}

bool TexoProducerMarkdown::Quote()
{
    bool ok = Close();
    quoted  = true;
    return ok && exporter.Put("> ");
}


bool TexoProducerMarkdown::Mono()
{
    return Mod("`");
}
bool TexoProducerMarkdown::Bold()
{
    return Mod("**");
}
bool TexoProducerMarkdown::Italic()
{
    return Mod("_");
}
bool TexoProducerMarkdown::Underline()
{
    return Mod("++");
}
bool TexoProducerMarkdown::Strike()
{
    return Mod("~~");
}

bool TexoProducerMarkdown::Link(
    const ScriptVariable & link, const ScriptVariable & title)
{
    bool ok = Link();
    if (link != "") {
        ok         = ok && exporter.Put('[');
        link_link  = link;
        link_title = title;
    }
    return ok;
}

bool TexoProducerMarkdown::Link()
{
    bool ok = true;
    if (link_link != "") {
        ok = ok && exporter.Put("](");
        ok = ok && exporter.Put(link_link);
        if (link_title != "") {
            ok = ok && exporter.Put(" \"");
            ok = ok && exporter.Put(link_title);
            ok = ok && exporter.Put('"');
        }
        ok = ok && exporter.Put(')');
    }
    return ok;
}


bool TexoProducerMarkdown::PutImage(const ScriptVariable & src,
    const ScriptVariable & alt, const ScriptVariable & title)
{
    bool ok = true;
    if (src != "") {
        ok = ok && exporter.Put("![");
        ok = ok && exporter.Put(alt);
        ok = ok && exporter.Put("](");
        ok = ok && exporter.Put(src);
        if (title != "") {
            ok = ok && exporter.Put(" \"");
            ok = ok && exporter.Put(title);
            ok = ok && exporter.Put('"');
        }
        ok = ok && exporter.Put(')');
    }
    newline = false;
    nospace = false;
    return ok;
}

bool TexoProducerMarkdown::PutHorizontalRule()
{
    bool ok = true;
    if (!newline && !nospace) {
        ok = ok && exporter.Put('\n');
    }
    nospace = true;
    return ok
        && exporter.Put(
            "--------------------------------------------------\n");
}


bool TexoProducerMarkdown::Mod(const ScriptVariable & str)
{
    newline = false;
    nospace = false;
    return exporter.Put(str);
}

bool TexoProducerMarkdown::Close()
{
    bool ok = true;
    if (code) {
        ok      = exporter.Put("\n```");
        code    = false;
        nospace = false;
        newline = false;
    }
    if (!newline && !nospace) {
        ok = ok && exporter.Put("\n\n");
    }
    header  = false;
    quoted  = false;
    nospace = true;
    return ok;
}


TexoImporterMarkdown::TexoImporterMarkdown(TexoProducer & producer):
    TexoImporter(producer), state(start)
{}


bool TexoImporterMarkdown::TruePut(char c)
{
    switch (state) {
    case start:
        Start(c);
        break;
    case text:
        Text(c);
        break;
    case header_text:
        HeaderText(c);
        break;
    case quote_pre:
        QuotePre(c);
        break;
    case quote_text:
        QuoteText(c);
        break;
    case quote_newline:
        QuoteNewline(c);
        break;
    case code_text:
        CodeText(c);
        break;
    case code_newline:
        CodeNewline(c);
        break;
    case code_end:
        CodeEnd(c);
        break;
    case backslash:
        Backslash(c);
        break;
    case asterisk:
        Asterisk(c);
        break;
    case underline:
        Underline(c);
        break;
    case plus:
        Plus(c);
        break;
    case tilde:
        Tilde(c);
        break;
    case newline:
        Newline(c);
        break;
    case rule:
        Rule(c);
        break;
    case paragraph:
        Paragraph(c);
        break;
    case header:
        Header(c);
        break;
    case header_pre:
        HeaderPre(c);
        break;
    case code:
        Code(c);
        break;
    }
    return ok;
}


void TexoImporterMarkdown::Start(char c)
{
    if (c == '\n') {
        state = paragraph;
    } else if (c == '#') {
        state        = header;
        header_level = 6;
    } else if (c == '>') {
        ok    = producer.Quote();
        state = quote_pre;
    } else if (c == '-') {
        ok              = producer.Paragraph();
        state           = rule;
        rule_dash_count = 1;
    } else if (c == '`') {
        state            = code;
        code_quote_count = 1;
    } else {
        ok    = producer.Paragraph();
        state = text;
        Put(c);
    }
}

void TexoImporterMarkdown::Text(char c)
{
    back = text;
    switch (c) {
    case '\\':
        state = backslash;
        break;
    case '\n':
        state = newline;
        break;
    case '*':
        state = asterisk;
        break;
    case '_':
        state = underline;
        break;
    case '+':
        state = plus;
        break;
    case '~':
        state = tilde;
        break;
    case '`':
        Backquote();
        break;
    default:
        ok = producer.Put(c);
    }
}

void TexoImporterMarkdown::HeaderText(char c)
{
    back = header_text;
    switch (c) {
    case '\\':
        state = backslash;
        break;
    case '\n':
        state = paragraph;
        break;
    case '*':
        state = asterisk;
        break;
    case '_':
        state = underline;
        break;
    case '+':
        state = plus;
        break;
    case '~':
        state = tilde;
        break;
    case '`':
        Backquote();
        break;
    default:
        ok = producer.Put(c);
    }
}

void TexoImporterMarkdown::QuotePre(char c)
{
    if (c != ' ') {
        state = quote_text;
        QuoteText(c);
    }
}

void TexoImporterMarkdown::QuoteText(char c)
{
    back = quote_text;
    switch (c) {
    case '\\':
        state = backslash;
        break;
    case '\n':
        state = quote_newline;
        break;
    case '*':
        state = asterisk;
        break;
    case '_':
        state = underline;
        break;
    case '+':
        state = plus;
        break;
    case '~':
        state = tilde;
        break;
    case '`':
        Backquote();
        break;
    default:
        ok = producer.Put(c);
    }
}

void TexoImporterMarkdown::QuoteNewline(char c)
{
    if (c == '>') {
        ok    = producer.Put('\n');
        state = quote_pre;
    } else {
        state = paragraph;
        Paragraph(c);
    }
}

void TexoImporterMarkdown::CodeText(char c)
{
    back = code_text;
    switch (c) {
    case '\\':
        state = backslash;
        break;
    case '\n':
        state = code_newline;
        break;
    case '*':
        state = asterisk;
        break;
    case '_':
        state = underline;
        break;
    case '+':
        state = plus;
        break;
    case '~':
        state = tilde;
        break;
    default:
        ok = producer.Put(c);
    }
}

void TexoImporterMarkdown::CodeNewline(char c)
{
    if (c == '`') {
        state            = code_end;
        code_quote_count = 1;
    } else {
        ok    = producer.Put('\n');
        state = code_text;
        Put(c);
    }
}

void TexoImporterMarkdown::CodeEnd(char c)
{
    if (c == '`') {
        ++code_quote_count;
    } else if (c == '\n') {
        if (code_quote_count == 3) {
            state = paragraph;
        } else {
            ok = false;
        }
    } else {
        ok = false;
    }
}

void TexoImporterMarkdown::Backslash(char c)
{
    ok    = producer.Put(c);
    state = back;
}

void TexoImporterMarkdown::Asterisk(char c)
{
    state = back;
    if (c == '*') {
        ok = producer.Bold();
    } else {
        ok = producer.Italic();
        Put(c);
    }
}

void TexoImporterMarkdown::Underline(char c)
{
    state = back;
    if (c == '_') {
        ok = producer.Bold();
    } else {
        ok = producer.Italic();
        Put(c);
    }
}

void TexoImporterMarkdown::Plus(char c)
{
    state = back;
    if (c == '+') {
        ok = producer.Underline();
    } else {
        ok = producer.Put('+');
        Put(c);
    }
}

void TexoImporterMarkdown::Tilde(char c)
{
    state = back;
    if (c == '~') {
        ok = producer.Strike();
    } else {
        ok = producer.Put('~');
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
        ok    = producer.Quote();
        state = quote_pre;
    } else if (c == '-') {
        state           = rule;
        rule_dash_count = 1;
    } else if (c == '`') {
        state            = code;
        code_quote_count = 1;
    } else {
        ok    = producer.Put('\n');
        state = text;
        Put(c);
    }
}

void TexoImporterMarkdown::Rule(char c)
{
    if (c == '-') {
        ++rule_dash_count;
    } else if (c == '\n' && rule_dash_count >= 3) {
        ok    = producer.PutHorizontalRule();
        state = newline;
    } else {
        ok = producer.Put(' ');
        for (int i = 0; ok && i < rule_dash_count; ++i) {
            ok = producer.Put('-');
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
        ok    = producer.Quote();
        state = quote_pre;
    } else if (c == '-') {
        ok              = producer.Paragraph();
        state           = rule;
        rule_dash_count = 1;
    } else if (c == '`') {
        state            = code;
        code_quote_count = 1;
    } else if (c != '\n') {
        ok    = producer.Paragraph();
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
            ok = false;
        }
    } else if (c == ' ') {
        ok    = producer.Header(header_level);
        state = header_pre;
    } else {
        ok = false;
    }
}

void TexoImporterMarkdown::HeaderPre(char c)
{
    if (c != ' ') {
        state = header_text;
        HeaderText(c);
    }
}

void TexoImporterMarkdown::Code(char c)
{
    if (c == '`') {
        ++code_quote_count;
    } else if (c == '\n') {
        if (code_quote_count == 3) {
            ok    = producer.Code();
            state = code_text;
        } else {
            ok = false;
        }
    } else {
        ok = false;
    }
}

void TexoImporterMarkdown::Backquote()
{
    ok = producer.Mono();
}
