#include "texo.hpp"


Texo::Texo(char c): c(c)
{}

TexoHeader::TexoHeader(int level, bool closing): level(level), closing(closing)
{}

TexoParagraph::TexoParagraph(bool closing): closing(closing)
{}

TexoCode::TexoCode(bool closing): closing(closing)
{}

TexoQuote::TexoQuote(bool closing): closing(closing)
{}

TexoMono::TexoMono(bool closing): closing(closing)
{}

TexoBold::TexoBold(bool closing): closing(closing)
{}

TexoItalic::TexoItalic(bool closing): closing(closing)
{}

TexoUnderline::TexoUnderline(bool closing): closing(closing)
{}

TexoStrike::TexoStrike(bool closing): closing(closing)
{}
