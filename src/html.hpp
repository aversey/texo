#ifndef TEXO_INCLUDED_HTML
#define TEXO_INCLUDED_HTML

#include "exporter.hpp"
#include "producer.hpp"


class TexoProducerHTML: public TexoProducerStrict {
public:
    TexoProducerHTML(TexoExporter &exporter);


protected:
    bool TruePut(char c);


    bool StartHeader(int level);
    bool CloseHeader(int level);

    bool StartCode();
    bool CloseCode();

    bool StartParagraph();
    bool CloseParagraph();

    bool StartQuote();
    bool CloseQuote();


    bool StartBold();
    bool CloseBold();

    bool StartItalic();
    bool CloseItalic();

    bool StartMono();
    bool CloseMono();

    bool StartStrike();
    bool CloseStrike();

    bool StartUnderline();
    bool CloseUnderline();

    bool StartLink(const char *link, const char *title);
    bool CloseLink(const char *link, const char *title);


    bool TruePutImage(const char *src, const char *alt, const char *title);

    bool TruePutHorizontalRule();
};


#endif
