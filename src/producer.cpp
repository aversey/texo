#include "producer.hpp"


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Texo Producer
 */
TexoProducer::TexoProducer(TexoExporter & exporter): exporter(exporter)
{}  // Just save exporter for future use.


bool TexoProducer::End()
{  // By default, no checks performed, so no error produced.
    return true;
}


bool TexoProducer::Put(char c)
{  // By default characters are not escaped, error means error in exporter.
    return exporter.Put(c);
}


// Block Signal Handlers
// By default, nothing has to be done, and no error produced,
// except for paragraphs, which must be implemented.
bool TexoProducer::Code()
{
    return true;
}

bool TexoProducer::Header(int level)
{
    return true;
}

bool TexoProducer::Quote()
{
    return true;
}


// Modificator Signal Handlers
// By default, nothing has to be done, and no error produced.
bool TexoProducer::Bold()
{
    return true;
}

bool TexoProducer::Italic()
{
    return true;
}

bool TexoProducer::Mono()
{
    return true;
}

bool TexoProducer::Strike()
{
    return true;
}

bool TexoProducer::Underline()
{
    return true;
}

bool TexoProducer::Link(
    const ScriptVariable & link, const ScriptVariable & title)
{
    return true;
}

bool TexoProducer::Link()
{
    return true;
}


bool TexoProducer::PutImage(const ScriptVariable & src,
    const ScriptVariable & alt, const ScriptVariable & title)
{
    return true;
}

bool TexoProducer::PutHorizontalRule()
{
    return true;
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Texo Strict Producer
 */
TexoProducerStrict::TexoProducerStrict(TexoExporter & exporter):
    TexoProducer(exporter), block(block_none), opened(0)
{}


bool TexoProducerStrict::End()
{
    bool ok = CloseBlock();
    block   = block_none;
    return ok;
}


bool TexoProducerStrict::Put(char c)
{
    return Start() && TruePut(c);
}


bool TexoProducerStrict::Code()
{
    if (CloseBlock()) {
        block = code;
        return StartCode();
    } else {
        return false;
    }
}

bool TexoProducerStrict::Header(int level)
{
    if (CloseBlock()) {
        block        = header;
        header_level = level;
        return StartHeader(level);
    } else {
        return false;
    }
}

bool TexoProducerStrict::Paragraph()
{
    if (CloseBlock()) {
        block = paragraph;
        return StartParagraph();
    } else {
        return false;
    }
}

bool TexoProducerStrict::Quote()
{
    if (CloseBlock()) {
        block = quote;
        return StartQuote();
    } else {
        return false;
    }
}


bool TexoProducerStrict::Bold()
{
    return SwitchMod(bold);
}

bool TexoProducerStrict::Italic()
{
    return SwitchMod(italic);
}

bool TexoProducerStrict::Mono()
{
    return SwitchMod(mono);
}

bool TexoProducerStrict::Strike()
{
    return SwitchMod(strike);
}

bool TexoProducerStrict::Underline()
{
    return SwitchMod(underline);
}

bool TexoProducerStrict::Link(
    const ScriptVariable & path, const ScriptVariable & title)
{
    if (IsOpened(link)) {
        int closed = CloseMods(link);
        if (closed >= 0) {
            return Reopen(closed);
        } else {
            return false;
        }
    } else {
        link_link  = &path;
        link_title = &title;
        return Start() && OpenMod(link);
    }
}

bool TexoProducerStrict::Link()
{
    if (IsOpened(link)) {
        int closed = CloseMods(link);
        if (closed >= 0) {
            return Reopen(closed);
        } else {
            return false;
        }
    }
    return true;
}

bool TexoProducerStrict::PutImage(const ScriptVariable & src,
    const ScriptVariable & alt, const ScriptVariable & title)
{
    return Start() && TruePutImage(src, alt, title);
}

bool TexoProducerStrict::PutHorizontalRule()
{
    return Start() && TruePutHorizontalRule();
}


bool TexoProducerStrict::Start()
{
    if (!block) {
        block = paragraph;
        return StartParagraph();
    } else {
        return true;
    }
}

bool TexoProducerStrict::CloseBlock()
{
    if (CloseMods()) {
        switch (block) {
        case block_none:
            return true;
        case code:
            return CloseCode();
        case header:
            return CloseHeader(header_level);
        case paragraph:
            return CloseParagraph();
        case quote:
            return CloseQuote();
        }
        return true;  // Inpossible, since all cases are in switch.
    } else {
        return false;
    }
}

bool TexoProducerStrict::SwitchMod(Mod mod)
{
    if (IsOpened(mod)) {
        int closed = CloseMods(mod);
        if (closed >= 0) {
            return Reopen(closed);
        } else {
            return false;
        }
    } else {
        return Start() && OpenMod(mod);
    }
}

bool TexoProducerStrict::OpenMod(Mod mod)
{
    mods[opened] = mod;
    ++opened;
    switch (mod) {
    case bold:
        return StartBold();
    case italic:
        return StartItalic();
    case link:
        return StartLink(*link_link, *link_title);
    case mono:
        return StartMono();
    case strike:
        return StartStrike();
    case underline:
        return StartUnderline();
    }
    return true;  // Inpossible, since all cases are in switch.
}

bool TexoProducerStrict::CloseMod(Mod mod)
{
    switch (mod) {
    case bold:
        return CloseBold();
    case italic:
        return CloseItalic();
    case link:
        return CloseLink(*link_link, *link_title);
    case mono:
        return CloseMono();
    case strike:
        return CloseStrike();
    case underline:
        return CloseUnderline();
    }
    return true;  // Inpossible, since all cases are in switch.
}

int TexoProducerStrict::CloseMods(Mod last)
{
    const int old = opened;
    while (opened) {
        Mod top = mods[--opened];
        if (CloseMod(top)) {
            if (top == last) {
                return old - opened;
            }
        } else {
            return -1;
        }
    }
    return old - opened;
}

bool TexoProducerStrict::CloseMods()
{
    bool ok = true;
    while (ok && opened) {
        ok = CloseMod(mods[--opened]);
    }
    return ok;
}

bool TexoProducerStrict::Reopen(int closed)
{
    bool ok         = true;
    int  new_opened = opened + closed;
    while (ok && opened != new_opened) {
        ok = OpenMod(mods[opened]);
    }
    return ok;
}

bool TexoProducerStrict::IsOpened(Mod mod) const
{
    bool is = false;
    for (int i = opened - 1; !is && i >= 0; --i) {
        is = mods[i] == mod;
    }
    return is;
}
