#ifndef TEXO_INCLUDED_HTML
#define TEXO_INCLUDED_HTML

#include "exporter.hpp"
#include "importer.hpp"
#include "producer.hpp"


class TexoProducerHTML: public TexoProducerStrict {
public:
    TexoProducerHTML(TexoExporter & exporter);

protected:
    bool TruePut(char c);

    bool StartCode();
    bool StartHeader(int level);
    bool StartParagraph();
    bool StartQuote();

    bool CloseCode();
    bool CloseHeader(int level);
    bool CloseParagraph();
    bool CloseQuote();

    bool StartBold();
    bool StartItalic();
    bool StartMono();
    bool StartStrike();
    bool StartUnderline();

    bool CloseBold();
    bool CloseItalic();
    bool CloseMono();
    bool CloseStrike();
    bool CloseUnderline();

    bool StartLink(const ScriptVariable & link, const ScriptVariable & title);
    bool CloseLink(const ScriptVariable & link, const ScriptVariable & title);

    bool TruePutImage(const ScriptVariable & src, const ScriptVariable & alt,
        const ScriptVariable & title);
    bool TruePutHorizontalRule();
};


#endif
