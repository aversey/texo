#include "producer.hpp"


TexoProducer::TexoProducer(TexoExporter &exporter): exporter(exporter)
{}

void TexoProducer::Put(char c)
{
    Put(Texo(c));
}

void TexoProducer::Put(const Texo &piece)
{
    exporter.Put(piece.c);
}

void TexoProducer::Put(const TexoHeader &piece)          {}
void TexoProducer::Put(const TexoParagraph &piece)       {}
void TexoProducer::Put(const TexoCode &piece)            {}
void TexoProducer::Put(const TexoQuote &piece)           {}

void TexoProducer::Put(const TexoMono &piece)            {}
void TexoProducer::Put(const TexoBold &piece)            {}
void TexoProducer::Put(const TexoItalic &piece)          {}
void TexoProducer::Put(const TexoUnderline &piece)       {}
void TexoProducer::Put(const TexoStrike &piece)          {}
void TexoProducer::Put(const TexoLink &piece)            {}

void TexoProducer::Put(const TexoImage &piece)           {}
void TexoProducer::Put(const TexoLineBreak &piece)       {}
void TexoProducer::Put(const TexoHorizontalRule &piece)  {}
