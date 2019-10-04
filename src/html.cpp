#include "html.hpp"

#include <string.h>


TexoHTMLProducer::TexoHTMLProducer(TexoExporter &exporter):
    TexoProducer(exporter)
{}

void TexoHTMLProducer::Put(const Texo &piece)
{
    switch (piece.type) {
    case Texo::character:       exporter.Put(piece.c);     break;
    case Texo::paragraph_begin: exporter.PutStr("<p>");    break;
    case Texo::paragraph_end:   exporter.PutStr("</p>");   break;
    case Texo::newline:         exporter.PutStr("<br/>");  break;
    case Texo::bold_begin:      exporter.PutStr("<b>");    break;
    case Texo::bold_end:        exporter.PutStr("</b>");   break;
    case Texo::italic_begin:    exporter.PutStr("<i>");    break;
    case Texo::italic_end:      exporter.PutStr("</i>");   break;
    case Texo::strike_begin:    exporter.PutStr("<del>");  break;
    case Texo::strike_end:      exporter.PutStr("</del>"); break;
    case Texo::underline_begin: exporter.PutStr("<ins>");  break;
    case Texo::underline_end:   exporter.PutStr("</ins>"); break;
    case Texo::link_begin:      BeginLink(piece);          break;
    case Texo::link_end:        exporter.PutStr("</a>");   break;
    case Texo::image:           Image(piece);              break;
    default:                                               break;
    }
}

void TexoHTMLProducer::BeginLink(const Texo &piece)
{
    exporter.PutStr("<a");
    if (piece.link_url) {
        exporter.PutStr(" href='");
        exporter.PutStr(piece.link_url);
        exporter.Put('\'');
    }
    exporter.Put('>');
}

void TexoHTMLProducer::Image(const Texo &piece)
{
    exporter.PutStr("<img");
    if (piece.image_src) {
        exporter.PutStr(" src='");
        exporter.PutStr(piece.image_src);
        exporter.Put('\'');
    }
    if (piece.image_alt) {
        exporter.PutStr(" alt='");
        exporter.PutStr(piece.image_alt);
        exporter.Put('\'');
    }
    exporter.PutStr("/>");
}


TexoHTMLStack::TexoHTMLStack(): next(0)
{}

TexoHTMLStack::~TexoHTMLStack()
{
    if (next) {
        while (next != this) {
            Pop();
        }
    }
}

void TexoHTMLStack::Push(Texo::Type tag)
{
    if (next) {
        TexoHTMLStack *buf = next;
        next               = new TexoHTMLStack;
        next->next         = buf;
        next->tag          = this->tag;
    } else {
        next = this;
    }
    this->tag = tag;
}

Texo::Type TexoHTMLStack::Pop()
{
    if (next) {
        Texo::Type res = tag;
        if (next != this) {
            TexoHTMLStack *buf = next;
            tag  = next->tag;
            next = next->next;
            delete buf;
        } else {
            next = 0;
        }
        return res;
    } else {
        return Texo::character;
    }
}


TexoHTMLImporter::TexoHTMLImporter(TexoProducer &producer, bool is_autoclose):
    TexoImporter(producer), state(text), is_closing(0), tag(Texo::character),
    readed(0), readed_len(0), readed_size(0), is_autoclose(is_autoclose)
{}

TexoHTMLImporter::~TexoHTMLImporter()
{
    ReadedClear();
}

void TexoHTMLImporter::Put(const char c)
{
    if (c) {
        switch (state) {
        case text:                   state = Text(c);                break;
        case tag_open:               state = TagOpen(c);             break;
        case tag_name:               state = TagName(c);             break;
        case tag_param_skip:         state = TagParamSkip(c);        break;
        case tag_param_pre:          state = TagParamPre(c);         break;
        case tag_param:              state = TagParam(c);            break;
        case tag_param_post:         state = TagParamPost(c);        break;
        case tag_param_value_pre:    state = TagParamValuePre(c);    break;
        case tag_param_value:        state = TagParamValue(c);       break;
        case tag_param_value_quoted: state = TagParamValueQuoted(c); break;
        default:                                                     break;
        }
    } else {
        tag = Texo::character;
        Close();
    }
}

TexoHTMLImporter::State TexoHTMLImporter::Text(const char c)
{
    State st = text;
    switch (c) {
    case '<': st = tag_open;           break;
    case '>': producer.PutStr("&gt;"); break;
    default:  producer.Put(Texo(c));   break;
    }
    return st;
}

TexoHTMLImporter::State TexoHTMLImporter::TagOpen(const char c)
{
    State st = tag_open;
    switch (c) {
    case '/':
        is_closing = 1;
        break;
    case ' ':
        st = text;
        producer.PutStr("&lt;");
        if (is_closing) {
            producer.Put(Texo('/'));
        }
        producer.Put(Texo(' '));
        break;
    case '>':
        st = text;
        producer.PutStr("&lt;");
        if (is_closing) {
            producer.Put(Texo('/'));
        }
        producer.PutStr("&gt;");
        break;
    default:
        st = tag_name;
        ReadedAdd(c);
        break;
    }
    return st;
}

TexoHTMLImporter::State TexoHTMLImporter::TagName(const char c)
{
    State st = tag_name;
    switch (c) {
    case ' ':
        st = tag_param_skip;
        SetTag();
        break;
    case '/':
        st = tag_param_skip;
        SetTag();
        break;
    case '>':
        st = text;
        SetTag();
        if (tag) {
            if (is_closing) {
                Close();
            } else {
                producer.Put(Texo(tag));
            }
        }
        is_closing = false;
        break;
    default:
        ReadedAdd(c);
        break;
    }
    return st;
}

TexoHTMLImporter::State TexoHTMLImporter::TagParamSkip(const char c)
{
    State st = tag_param_skip;
    switch (c) {
    case '>':
        st = text;
        if (tag) {
            if (is_closing) {
                Close();
            } else {
                producer.Put(Texo(tag));
            }
        }
        is_closing = false;
        break;
    default:
        break;
    }
    return st;
}

TexoHTMLImporter::State TexoHTMLImporter::TagParamPre(const char c)
{
    return text;
}

TexoHTMLImporter::State TexoHTMLImporter::TagParam(const char c)
{
    return text;
}

TexoHTMLImporter::State TexoHTMLImporter::TagParamPost(const char c)
{
    return text;
}

TexoHTMLImporter::State TexoHTMLImporter::TagParamValuePre(const char c)
{
    return text;
}

TexoHTMLImporter::State TexoHTMLImporter::TagParamValue(const char c)
{
    return text;
}

TexoHTMLImporter::State TexoHTMLImporter::TagParamValueQuoted(const char c)
{
    return text;
}

void TexoHTMLImporter::Close()
{
    Texo::Type current = opened.Pop();
    while ((current != tag) && current) {
        producer.Put(Texo(current));
        current = opened.Pop();
    }
    if (current) {
        producer.Put(Texo(current));
    }
}

void TexoHTMLImporter::SetTag()
{
    if (!strcmp(readed, "a")) {
        if (is_closing) {
            tag = Texo::link_end;
        } else {
            tag = Texo::link_begin;
            opened.Push(Texo::link_end);
        }
    } else if (!strcmp(readed, "img")) {
        tag = Texo::image;
    } else if (!strcmp(readed, "b")) {
        if (is_closing) {
            tag = Texo::bold_end;
        } else {
            tag = Texo::bold_begin;
            opened.Push(Texo::bold_end);
        }
    } else if (!strcmp(readed, "i")) {
        if (is_closing) {
            tag = Texo::italic_end;
        } else {
            tag = Texo::italic_begin;
            opened.Push(Texo::italic_end);
        }
    } else if (!strcmp(readed, "del")) {
        if (is_closing) {
            tag = Texo::strike_end;
        } else {
            tag = Texo::strike_begin;
            opened.Push(Texo::strike_end);
        }
    } else if (!strcmp(readed, "ins")) {
        if (is_closing) {
            tag = Texo::underline_end;
        } else {
            tag = Texo::underline_begin;
            opened.Push(Texo::underline_end);
        }
    } else if (!strcmp(readed, "p")) {
        if (is_closing) {
            tag = Texo::paragraph_end;
        } else {
            tag = Texo::paragraph_begin;
            opened.Push(Texo::paragraph_end);
        }
    } else if (!strcmp(readed, "br")) {
        tag = Texo::newline;
    } else {
        tag = Texo::character;
    }
    ReadedClear();
}

void TexoHTMLImporter::ReadedAdd(const char c)
{
    if (readed_size == 0) {
        readed_size = 2;
        readed      = new char[readed_size];
    } else if (readed_size - 1 == readed_len) {
        char *buf = readed;
        readed_size = readed_size << 1;
        readed      = new char[readed_size];
        for (int i = 0; i < readed_len; ++i) {
            readed[i] = buf[i];
        }
        delete buf;
    }
    ++readed_len;
    readed[readed_len - 1] = c;
    readed[readed_len]     = 0;
}

void TexoHTMLImporter::ReadedClear()
{
    readed_len  = 0;
    readed_size = 0;
    if (readed) {
        delete readed;
        readed = 0;
    }
}
