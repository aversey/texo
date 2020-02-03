#include "markdown.hpp"


TexoProducerMarkdown::TexoProducerMarkdown(TexoExporter &exporter):
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
    if (!Close()) {
        return false;
    }
    header = true;
    switch (level) {
    case 1:  return exporter.Put("###### ");
    case 2:  return exporter.Put("##### ");
    case 3:  return exporter.Put("#### ");
    case 4:  return exporter.Put("### ");
    case 5:  return exporter.Put("## ");
    default: return exporter.Put("# ");
    }
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


bool TexoProducerMarkdown::Mono()       { return Mod("`"); }
bool TexoProducerMarkdown::Bold()       { return Mod("**"); }
bool TexoProducerMarkdown::Italic()     { return Mod("_"); }
bool TexoProducerMarkdown::Underline()  { return Mod("++"); }
bool TexoProducerMarkdown::Strike()     { return Mod("~~"); }

bool TexoProducerMarkdown::Link(const char *link, const char *title)
{
    bool ok = Link();
    if (link) {
        ok         = ok && exporter.Put('[');
        link_link  = link;
        link_title = title;
    }
    return ok;
}

bool TexoProducerMarkdown::Link()
{
    bool ok = true;
    if (link_link) {
        ok = ok && exporter.Put("](");
        ok = ok && exporter.Put(link_link);
        if (link_title) {
            ok = ok && exporter.Put(" \"");
            ok = ok && exporter.Put(link_title);
            ok = ok && exporter.Put('"');
        }
        ok = ok && exporter.Put(')');
    }
    return ok;
}


bool TexoProducerMarkdown::PutImage(const char *src,
                                    const char *alt,
                                    const char *title)
{
    bool ok = true;
    if (src) {
        ok = ok && exporter.Put("![");
        ok = ok && exporter.Put(alt);
        ok = ok && exporter.Put("](");
        ok = ok && exporter.Put(src);
        if (title) {
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
    return ok && exporter.Put("------------------------------------------\n");
}


bool TexoProducerMarkdown::Mod(const char *str)
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


TexoImporterMarkdown::TexoImporterMarkdown(TexoProducer &producer):
    TexoImporter(producer), state(start)
{}


bool TexoImporterMarkdown::TruePut(char c)
{
    switch (state) {
    case start:         return Start(c);
    case text:          return Text(c);
    case header_text:   return HeaderText(c);
    case quote_pre:     return QuotePre(c);
    case quote_text:    return QuoteText(c);
    case quote_newline: return QuoteNewline(c);
    case code_text:     return CodeText(c);
    case code_newline:  return CodeNewline(c);
    case code_end:      return CodeEnd(c);
    case backslash:     return Backslash(c);
    case asterisk:      return Asterisk(c);
    case underline:     return Underline(c);
    case plus:          return Plus(c);
    case tilde:         return Tilde(c);
    case newline:       return Newline(c);
    case rule:          return Rule(c);
    case paragraph:     return Paragraph(c);
    case header:        return Header(c);
    case header_pre:    return HeaderPre(c);
    case code:          return Code(c);
    }
    return ok;  // Impossible
}


bool TexoImporterMarkdown::Start(char c)
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
    return ok;
}

bool TexoImporterMarkdown::Text(char c)
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
    default:   ok = producer.Put(c);
    }
    return ok;
}

bool TexoImporterMarkdown::HeaderText(char c)
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
    default:   ok = producer.Put(c);
    }
    return ok;
}

bool TexoImporterMarkdown::QuotePre(char c)
{
    if (c != ' ') {
        state = quote_text;
        QuoteText(c);
    }
    return ok;
}

bool TexoImporterMarkdown::QuoteText(char c)
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
    default:   ok = producer.Put(c);
    }
    return ok;
}

bool TexoImporterMarkdown::QuoteNewline(char c)
{
    if (c == '>') {
        ok    = producer.Put('\n');
        state = quote_pre;
    } else {
        state = paragraph;
        Paragraph(c);
    }
    return ok;
}

bool TexoImporterMarkdown::CodeText(char c)
{
    back = code_text;
    switch (c) {
    case '\\': state = backslash;    break;
    case '\n': state = code_newline; break;
    case '*':  state = asterisk;     break;
    case '_':  state = underline;    break;
    case '+':  state = plus;         break;
    case '~':  state = tilde;        break;
    default:   ok = producer.Put(c);
    }
    return ok;
}

bool TexoImporterMarkdown::CodeNewline(char c)
{
    if (c == '`') {
        state            = code_end;
        code_quote_count = 1;
    } else {
        ok    = producer.Put('\n');
        state = code_text;
        Put(c);
    }
    return ok;
}

bool TexoImporterMarkdown::CodeEnd(char c)
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
    return ok;
}

bool TexoImporterMarkdown::Backslash(char c)
{
    state = back;
    return ok = producer.Put(c);
}

bool TexoImporterMarkdown::Asterisk(char c)
{
    state = back;
    if (c == '*') {
        ok = producer.Bold();
    } else {
        ok = producer.Italic();
        Put(c);
    }
    return ok;
}

bool TexoImporterMarkdown::Underline(char c)
{
    state = back;
    if (c == '_') {
        ok = producer.Bold();
    } else {
        ok = producer.Italic();
        Put(c);
    }
    return ok;
}

bool TexoImporterMarkdown::Plus(char c)
{
    state = back;
    if (c == '+') {
        ok = producer.Underline();
    } else {
        ok = producer.Put('+');
        Put(c);
    }
    return ok;
}

bool TexoImporterMarkdown::Tilde(char c)
{
    state = back;
    if (c == '~') {
        ok = producer.Strike();
    } else {
        ok = producer.Put('~');
        Put(c);
    }
    return ok;
}

bool TexoImporterMarkdown::Newline(char c)
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
    return ok;
}

bool TexoImporterMarkdown::Rule(char c)
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
    return ok;
}

bool TexoImporterMarkdown::Paragraph(char c)
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
    return ok;
}

bool TexoImporterMarkdown::Header(char c)
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
    return ok;
}

bool TexoImporterMarkdown::HeaderPre(char c)
{
    if (c != ' ') {
        state = header_text;
        HeaderText(c);
    }
    return ok;
}

bool TexoImporterMarkdown::Code(char c)
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
    return ok;
}

void TexoImporterMarkdown::Backquote()
{
    ok = producer.Mono();
}
