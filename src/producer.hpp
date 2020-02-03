#ifndef TEXO_INCLUDED_PRODUCER
#define TEXO_INCLUDED_PRODUCER

#include "exporter.hpp"


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Texo Producer
 * Texo interface for producing output based on inner representation.
 */
class TexoProducer {
public:
    TexoProducer(TexoExporter &exporter);  // Setup exporter used for output.


    virtual bool End();  // Notify producer, what input is ended.


    virtual bool Put(char c);  // Output c, possibly escaping it.


    // Block Signal Handlers
    // Only one can be opened at one moment.
    virtual bool Header(int level);
    virtual bool Code();
    virtual bool Paragraph() = 0;
    virtual bool Quote();


    // Modificator Signal Handlers
    // Independently switch one of text modificators.
    virtual bool Bold();
    virtual bool Italic();
    virtual bool Mono();
    virtual bool Strike();
    virtual bool Underline();

    // Switch link to given or switch it off, if no one is given.
    virtual bool Link(const char *link, const char *title);
    virtual bool Link();


    virtual bool PutImage(const char *src, const char *alt, const char *title);
    virtual bool PutHorizontalRule();


protected:
    TexoExporter &exporter;
};


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Texo Strict Producer
 * Converts switch-signal interface of Texo Producer into
 * sequence of start/close signals with disjoint modificator sections,
 * as well as block sections, which are provided on top level.
 */
class TexoProducerStrict: public TexoProducer {
public:
    TexoProducerStrict(TexoExporter & exporter);


    bool End();


    bool Put(char c);


    bool Header(int level);
    bool Code();
    bool Paragraph();
    bool Quote();


    bool Bold();
    bool Italic();
    bool Mono();
    bool Strike();
    bool Underline();

    bool Link(const char *path, const char *title);
    bool Link();

    bool PutImage(const char *src, const char *alt, const char *title);

    bool PutHorizontalRule();


protected:
    virtual bool TruePut(char c) = 0;


    virtual bool StartHeader(int level) = 0;
    virtual bool CloseHeader(int level) = 0;

    virtual bool StartCode()            = 0;
    virtual bool CloseCode()            = 0;

    virtual bool StartParagraph()       = 0;
    virtual bool CloseParagraph()       = 0;

    virtual bool StartQuote()           = 0;
    virtual bool CloseQuote()           = 0;


    virtual bool StartBold()      = 0;
    virtual bool CloseBold()      = 0;

    virtual bool StartItalic()    = 0;
    virtual bool CloseItalic()    = 0;

    virtual bool StartMono()      = 0;
    virtual bool CloseMono()      = 0;

    virtual bool StartStrike()    = 0;
    virtual bool CloseStrike()    = 0;

    virtual bool StartUnderline() = 0;
    virtual bool CloseUnderline() = 0;

    virtual bool StartLink(const char *link, const char *title) = 0;
    virtual bool CloseLink(const char *link, const char *title) = 0;


    virtual bool TruePutImage(const char *src,
                              const char *alt,
                              const char *title) = 0;

    virtual bool TruePutHorizontalRule() = 0;


private:
    enum Block { block_none = 0, code, header, paragraph, quote } block;

    int header_level;


    enum Mod { bold, italic, link, mono, strike, underline } mods[6];
    int opened;

    const char **link_link;
    const char **link_title;


    bool Start();  // Open paragraph if no block is opened.

    bool CloseBlock();

    bool SwitchMod(Mod mod);

    bool OpenMod(Mod mod);

    bool CloseMod(Mod mod);

    // Close all opened mods sequentially, down to (including) last,
    // returning count of closed mods, or negative number in case of error.
    int CloseMods(Mod last);

    // Close all opened mods.
    bool CloseMods();

    bool Reopen(int closed);

    bool IsOpened(Mod mod) const;
};


#endif
