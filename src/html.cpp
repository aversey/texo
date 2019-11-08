#include "html.hpp"


TexoProducerHTML::TexoProducerHTML(TexoExporter &exporter):
    TexoProducerStrict(exporter)
{}


bool TexoProducerHTML::TruePut(char c)
{
    switch (c) {
    case '<': return exporter.Put("&lt;");  break;
    case '>': return exporter.Put("&gt;");  break;
    case '&': return exporter.Put("&amp;"); break;
    default:  return exporter.Put(c);
    }
}


bool TexoProducerHTML::StartCode()
{
    return exporter.Put("\n<pre>\n");
}

bool TexoProducerHTML::StartHeader(int level)
{
    if (level <= 1) {
        return exporter.Put("\n<h6>\n");
    } else if (level == 2) {
        return exporter.Put("\n<h5>\n");
    } else if (level == 3) {
        return exporter.Put("\n<h4>\n");
    } else if (level == 4) {
        return exporter.Put("\n<h3>\n");
    } else if (level == 5) {
        return exporter.Put("\n<h2>\n");
    } else {
        return exporter.Put("\n<h1>\n");
    }
}

bool TexoProducerHTML::StartParagraph()
{
    return exporter.Put("\n<p>\n");
}

bool TexoProducerHTML::StartQuote()
{
    return exporter.Put("\n<blockquote><p>\n");
}

bool TexoProducerHTML::CloseCode()
{
    return exporter.Put("\n</pre>\n");
}

bool TexoProducerHTML::CloseHeader(int level)
{
    if (level <= 1) {
        return exporter.Put("\n</h6>\n");
    } else if (level == 2) {
        return exporter.Put("\n</h5>\n");
    } else if (level == 3) {
        return exporter.Put("\n</h4>\n");
    } else if (level == 4) {
        return exporter.Put("\n</h3>\n");
    } else if (level == 5) {
        return exporter.Put("\n</h2>\n");
    } else {
        return exporter.Put("\n</h1>\n");
    }
}

bool TexoProducerHTML::CloseParagraph()
{
    return exporter.Put("\n</p>\n");
}

bool TexoProducerHTML::CloseQuote()
{
    return exporter.Put("\n</p></blockquote>\n");
}


bool TexoProducerHTML::StartBold()       { return exporter.Put("<b>"); }
bool TexoProducerHTML::StartItalic()     { return exporter.Put("<i>"); }
bool TexoProducerHTML::StartMono()       { return exporter.Put("<code>"); }
bool TexoProducerHTML::StartStrike()     { return exporter.Put("<del>"); }
bool TexoProducerHTML::StartUnderline()  { return exporter.Put("<ins>"); }

bool TexoProducerHTML::CloseBold()       { return exporter.Put("</b>"); }
bool TexoProducerHTML::CloseItalic()     { return exporter.Put("</i>"); }
bool TexoProducerHTML::CloseMono()       { return exporter.Put("</code>"); }
bool TexoProducerHTML::CloseStrike()     { return exporter.Put("</del>"); }
bool TexoProducerHTML::CloseUnderline()  { return exporter.Put("</ins>"); }

bool TexoProducerHTML::StartLink(
    const ScriptVariable &link,
    const ScriptVariable &title
)
{
    bool ok = true;
    if (link != "") {
        ok = ok && exporter.Put("<a href='");
        ok = ok && exporter.Put(link);
        if (title != "") {
            ok = ok && exporter.Put("' title='");
            ok = ok && exporter.Put(title);
        }
        ok = ok && exporter.Put("'>");
    }
    return ok;
}

bool TexoProducerHTML::CloseLink(
    const ScriptVariable &link,
    const ScriptVariable &title
)
{
    if (link != "") {
        return exporter.Put("</a>");
    } else {
        return true;
    }
}


bool TexoProducerHTML::TruePutImage(
    const ScriptVariable &src,
    const ScriptVariable &alt,
    const ScriptVariable &title
)
{
    bool ok = true;
    if (src != "") {
        ok = ok && exporter.Put("<img src='");
        ok = ok && exporter.Put(src);
        if (alt != "") {
            ok = ok && exporter.Put("' alt='");
            ok = ok && exporter.Put(alt);
        }
        if (title != "") {
            ok = ok && exporter.Put("' title='");
            ok = ok && exporter.Put(title);
        }
        ok = ok && exporter.Put("'/>");
    }
    return ok;
}

bool TexoProducerHTML::TruePutHorizontalRule()
{
    return exporter.Put("\n<hr/>\n");
}
