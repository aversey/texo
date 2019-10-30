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
    TexoImporter(producer), state(start), wrapping_state(error), mod_pos(-1)
{}

TexoImporterMarkdown::~TexoImporterMarkdown()
{
    EndBlock();
}

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
        BlockState(quote_text);
    } else if (c == '-') {
        BlockState(text);
        state           = rule;
        rule_dash_count = 1;
    } else if (c == '`') {
        state            = code;
        code_quote_count = 1;
    } else {
        BlockState(text);
        Put(c);
    }
}

void TexoImporterMarkdown::Text(char c)
{
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
        state = quote_text;
    } else {
        state = paragraph;
        Put(c);
    }
}

void TexoImporterMarkdown::CodeText(char c)
{
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
            Put(c);
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
    state = wrapping_state;
}

void TexoImporterMarkdown::Asterisk(char c)
{
    state = wrapping_state;
    if (c == '*') {
        if (CheckMods(bold)) {
            producer.Put(TexoBold(Mod(bold)));
        } else {
            state = error;
        }
    } else {
        if (CheckMods(italic)) {
            producer.Put(TexoItalic(Mod(italic)));
            Put(c);
        } else {
            state = error;
        }
    }
}

void TexoImporterMarkdown::Underline(char c)
{
    state = wrapping_state;
    if (c == '_') {
        if (CheckMods(bold)) {
            producer.Put(TexoBold(Mod(bold)));
        } else {
            state = error;
        }
    } else {
        if (CheckMods(italic)) {
            producer.Put(TexoItalic(Mod(italic)));
            Put(c);
        } else {
            state = error;
        }
    }
}

void TexoImporterMarkdown::Plus(char c)
{
    state = wrapping_state;
    if (c == '+') {
        if (CheckMods(underlined)) {
            producer.Put(TexoUnderline(Mod(underlined)));
        } else {
            state = error;
        }
    } else {
        producer.Put('+');
        Put(c);
    }
}

void TexoImporterMarkdown::Tilde(char c)
{
    state = wrapping_state;
    if (c == '~') {
        if (CheckMods(strike)) {
            producer.Put(TexoStrike(Mod(strike)));
        } else {
            state = error;
        }
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
        BlockState(quote_text);
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
        state = wrapping_state;
        Put(c);
    }
}

void TexoImporterMarkdown::Paragraph(char c)
{
    if (c == '#') {
        state        = header;
        header_level = 6;
    } else if (c == '>') {
        BlockState(quote_text);
    } else if (c == '-') {
        BlockState(text);
        state           = rule;
        rule_dash_count = 1;
    } else if (c == '`') {
        state            = code;
        code_quote_count = 1;
    } else {
        BlockState(text);
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
        BlockState(header_text);
        header_level_last = header_level;
        Put(c);
    }
}

void TexoImporterMarkdown::Code(char c)
{
    if (c == '`') {
        ++code_quote_count;
    } else  if (c == '\n') {
        if (code_quote_count == 3) {
            BlockState(code_text);
        } else {
            state = error;
        }
    } else {
        state = error;
    }
}

void TexoImporterMarkdown::Backquote()
{
    if (CheckMods(mono)) {
        producer.Put(TexoMono(Mod(mono)));
    } else {
        state = error;
    }
}

bool TexoImporterMarkdown::CheckMods(Modificator mod)
{
    bool ok = true;
    for (int i = 0; ok && i < mod_pos; ++i) {
        ok = mods[i] != mod;
    }
    return ok;
}

bool TexoImporterMarkdown::Mod(Modificator mod)
{
    if (mod_pos == -1 || mods[mod_pos] != mod) {
        ++mod_pos;
        mods[mod_pos] = mod;
        return false;
    }
    --mod_pos;
    return true;
}

void TexoImporterMarkdown::BlockState(State st)
{
    if (mod_pos == -1) {
        state = st;
        EndBlock();
        if (st == text) {
            producer.Put(TexoParagraph());
        } else if (st == quote_text) {
            producer.Put(TexoQuote());
        } else if (st == code_text) {
            producer.Put(TexoCode());
        } else if (st == header_text) {
            producer.Put(TexoHeader(header_level));
        } else {
            state = error;
        }
        wrapping_state = st;
    } else {
        state = error;
    }
}

void TexoImporterMarkdown::EndBlock()
{
    if (wrapping_state == text) {
        producer.Put(TexoParagraph(true));
    } else if (wrapping_state == quote_text) {
        producer.Put(TexoQuote(true));
    } else if (wrapping_state == code_text) {
        producer.Put(TexoCode(true));
    } else if (wrapping_state == header_text) {
        producer.Put(TexoHeader(header_level_last, true));
    } else if (wrapping_state != error) {
        state = error;
    }
}
