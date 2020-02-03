#include "html.hpp"


TexoProducerHTML::TexoProducerHTML(TexoExporter & exporter):
    TexoProducerStrict(exporter)
{}


bool TexoProducerHTML::TruePut(char c)
{
    switch (c) {
    case '<': return exporter.Put("&lt;");
    case '>': return exporter.Put("&gt;");
    case '&': return exporter.Put("&amp;");
    default:  return exporter.Put(c);
    }
}


bool TexoProducerHTML::StartHeader(int level)
{
    switch (level) {
    case 1:  return exporter.Put("\n<h6>\n");
    case 2:  return exporter.Put("\n<h5>\n");
    case 3:  return exporter.Put("\n<h4>\n");
    case 4:  return exporter.Put("\n<h3>\n");
    case 5:  return exporter.Put("\n<h2>\n");
    default: return exporter.Put("\n<h1>\n");
    }
}

bool TexoProducerHTML::CloseHeader(int level)
{
    switch (level) {
    case 1:  return exporter.Put("\n</h6>\n");
    case 2:  return exporter.Put("\n</h5>\n");
    case 3:  return exporter.Put("\n</h4>\n");
    case 4:  return exporter.Put("\n</h3>\n");
    case 5:  return exporter.Put("\n</h2>\n");
    default: return exporter.Put("\n</h1>\n");
    }
}

bool TexoProducerHTML::StartQuote()
{
    return exporter.Put("\n<blockquote><p>\n");
}

bool TexoProducerHTML::CloseQuote()
{
    return exporter.Put("\n</p></blockquote>\n");
}

bool TexoProducerHTML::StartCode()       { return exporter.Put("\n<pre>\n"); }
bool TexoProducerHTML::CloseCode()       { return exporter.Put("\n</pre>\n"); }

bool TexoProducerHTML::StartParagraph()  { return exporter.Put("\n<p>\n"); }
bool TexoProducerHTML::CloseParagraph()  { return exporter.Put("\n</p>\n"); }


bool TexoProducerHTML::StartBold()       { return exporter.Put("<b>"); }
bool TexoProducerHTML::CloseBold()       { return exporter.Put("</b>"); }

bool TexoProducerHTML::StartItalic()     { return exporter.Put("<i>"); }
bool TexoProducerHTML::CloseItalic()     { return exporter.Put("</i>"); }

bool TexoProducerHTML::StartMono()       { return exporter.Put("<code>"); }
bool TexoProducerHTML::CloseMono()       { return exporter.Put("</code>"); }

bool TexoProducerHTML::StartStrike()     { return exporter.Put("<del>"); }
bool TexoProducerHTML::CloseStrike()     { return exporter.Put("</del>"); }

bool TexoProducerHTML::StartUnderline()  { return exporter.Put("<ins>"); }
bool TexoProducerHTML::CloseUnderline()  { return exporter.Put("</ins>"); }

bool TexoProducerHTML::StartLink(const char *link, const char *title)
{
    if (!link) {
        return true;
    }
    bool ok = exporter.Put("<a href='") && exporter.Put(link);
    if (title) {
        ok = ok && exporter.Put("' title='") && exporter.Put(title);
    }
    return ok && exporter.Put("'>");
}

bool TexoProducerHTML::CloseLink(const char *link, const char *title)
{
    if (!link) {
        return true;
    }
    return exporter.Put("</a>");
}


bool TexoProducerHTML::TruePutImage(const char *src,
                                    const char *alt,
                                    const char *title)
{
    if (!src) {
        return true;
    }
    bool ok = exporter.Put("<img src='") && exporter.Put(src);
    if (alt) {
        ok = ok && exporter.Put("' alt='") && exporter.Put(alt);
    }
    if (title) {
        ok = ok && exporter.Put("' title='") && exporter.Put(title);
    }
    return ok && exporter.Put("'/>");
}

bool TexoProducerHTML::TruePutHorizontalRule()
{
    return exporter.Put("\n<hr/>\n");
}
