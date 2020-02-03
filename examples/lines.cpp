#include <file.hpp>
#include <html.hpp>
#include <lines.hpp>
#include <markdown.hpp>
#include <plain.hpp>
#include <stdio.h>


void plain()
{
    fputs("  ----  Lines to Plain  ----\n", stdout);
    TexoFile          exporter(stdout);
    TexoProducerPlain producer(exporter);
    TexoImporterLines importer(producer);
    FILE             *file = fopen("lines.txt", "r");
    importer.Put(file);
    importer.End();
    fclose(file);
}

void html()
{
    fputs("\n  ----  Lines to HTML  ----\n", stdout);
    TexoFile          exporter(stdout);
    TexoProducerHTML  producer(exporter);
    TexoImporterLines importer(producer);
    FILE             *file = fopen("lines.txt", "r");
    importer.Put(file);
    importer.End();
    fclose(file);
}

void markdown()
{
    fputs("\n  ----  Lines to Markdown  ----\n", stdout);
    TexoFile             exporter(stdout);
    TexoProducerMarkdown producer(exporter);
    TexoImporterLines    importer(producer);
    FILE                *file = fopen("lines.txt", "r");
    importer.Put(file);
    importer.End();
    fclose(file);
}

void lines()
{
    fputs("\n  ----  Lines to Lines  ----\n", stdout);
    TexoFile          exporter(stdout);
    TexoProducerLines producer(exporter);
    TexoImporterLines importer(producer);
    FILE             *file = fopen("lines.txt", "r");
    importer.Put(file);
    importer.End();
    fclose(file);
}


int main()
{
    plain();
    html();
    markdown();
    lines();

    return 0;
}
