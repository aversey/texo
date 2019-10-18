#ifndef TEXO_INCLUDED_PRODUCER
#define TEXO_INCLUDED_PRODUCER

#include "texo.hpp"
#include "exporter.hpp"


class TexoProducer {
public:
    TexoProducer(TexoExporter &exporter);

    virtual void Put(const Texo &piece);

    virtual void Put(const TexoHeader &piece);
    virtual void Put(const TexoParagraph &piece);
    virtual void Put(const TexoCode &piece);
    virtual void Put(const TexoQuote &piece);

    virtual void Put(const TexoMono &piece);
    virtual void Put(const TexoBold &piece);
    virtual void Put(const TexoItalic &piece);
    virtual void Put(const TexoUnderline &piece);
    virtual void Put(const TexoStrike &piece);

    virtual void Put(const TexoImage &piece);
    virtual void Put(const TexoLink &piece);
    virtual void Put(const TexoLineBreak &piece);
    virtual void Put(const TexoHorizontalRule &piece);

protected:
    TexoExporter &exporter;
};


#endif
