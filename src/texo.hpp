#ifndef TEXO_INCLUDED_TEXO
#define TEXO_INCLUDED_TEXO

#include <stdio.h>


class Texo {
public:
    enum Type {
        character = 0,
        link_begin,
        link_end,
        image,
        bold_begin,
        bold_end,
        italic_begin,
        italic_end,
        strike_begin,
        strike_end,
        underline_begin,
        underline_end,
        paragraph_begin,
        paragraph_end,
        newline
    } type;
    Texo(Type type);
    Texo(const char c);
    const char c;
    const char *link_url;
    const char *image_src;
    const char *image_alt;
};


class TexoExporter {
public:
    virtual void Put(const char c) = 0;
    virtual void PutStr(const char *str);
};


class TexoProducer {
public:
    TexoProducer(TexoExporter &exporter);
    virtual void Put(const Texo &piece) = 0;
protected:
    TexoExporter &exporter;
};


class TexoImporter {
public:
    TexoImporter(TexoProducer &producer);
    virtual void Put(const char c) = 0;
    virtual void PutStr(const char *str);
    virtual void PutFile(FILE *file);
protected:
    TexoProducer &producer;
};


#endif
