#ifndef TEXO_INCLUDED_HTML
#define TEXO_INCLUDED_HTML

#include "texo.hpp"


class TexoHTMLProducer: public TexoProducer {
public:
    TexoHTMLProducer(TexoExporter &exporter);
    void Put(const Texo &piece);

private:
    void BeginLink(const Texo &piece);
    void Image(const Texo &piece);
};


class TexoHTMLStack {
public:
    TexoHTMLStack();
    ~TexoHTMLStack();
    void Push(Texo::Type tag);
    Texo::Type Pop();

private:
    TexoHTMLStack *next;
    Texo::Type    tag;
};

class TexoHTMLImporter: public TexoImporter {
public:
    TexoHTMLImporter(TexoProducer &producer, bool is_autoclose = true);
    ~TexoHTMLImporter();
    void Put(const char c);

private:
    bool is_autoclose;

    enum State {
        text = 0,
        tag_open,
        tag_name,
        tag_param_skip,
        tag_param_pre,
        tag_param,
        tag_param_post,
        tag_param_value_pre,
        tag_param_value,
        tag_param_value_quoted
    } state;

    State Text(const char c);
    State TagOpen(const char c);
    State TagName(const char c);
    State TagParamSkip(const char c);
    State TagParamPre(const char c);
    State TagParam(const char c);
    State TagParamPost(const char c);
    State TagParamValuePre(const char c);
    State TagParamValue(const char c);
    State TagParamValueQuoted(const char c);

    TexoHTMLStack opened;

    void Close();

    bool       is_closing;
    Texo::Type tag;

    void SetTag();

    char *readed;
    int  readed_len;
    int  readed_size;

    void ReadedAdd(const char c);
    void ReadedClear();
};


#endif
