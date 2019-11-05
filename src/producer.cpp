#include "producer.hpp"


TexoProducer::TexoProducer(TexoExporter &exporter): exporter(exporter)
{}

void TexoProducer::End()
{}

void TexoProducer::Put(char c)
{
    Put(Texo(c));
}

void TexoProducer::Put(const Texo &piece)
{
    exporter.Put(piece.c);
}

void TexoProducer::Put(const TexoHeader &piece)
{
    Put(TexoParagraph());
}

void TexoProducer::Put(const TexoQuote &piece)
{
    Put(TexoParagraph());
}

void TexoProducer::Put(const TexoCode &piece)
{
    Put(TexoParagraph());
}

void TexoProducer::Put(const TexoMono &piece)            {}
void TexoProducer::Put(const TexoBold &piece)            {}
void TexoProducer::Put(const TexoItalic &piece)          {}
void TexoProducer::Put(const TexoUnderline &piece)       {}
void TexoProducer::Put(const TexoStrike &piece)          {}

void TexoProducer::Put(const TexoImage &piece)           {}
void TexoProducer::Put(const TexoLink &piece)            {}
void TexoProducer::Put(const TexoHorizontalRule &piece)  {}
