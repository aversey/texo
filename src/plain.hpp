#ifndef TEXO_INCLUDED_PLAIN
#define TEXO_INCLUDED_PLAIN

#include "texo.hpp"


class TexoPlainImporter: public TexoImporter {
public:
    TexoPlainImporter(TexoProducer &producer);
    void Put(const char c);

private:
    int  space;
    int  newline;
    bool first;
};


#endif
