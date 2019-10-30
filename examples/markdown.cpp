#include <stdio.h>
#include <html.hpp>
#include <plain.hpp>
#include <lines.hpp>
#include <markdown.hpp>
#include <file.hpp>


void plain()
{
    fputs("  ----  Markdown to Plain  ----\n\n", stdout);
    TexoExporterFile exporter(stdout);
    TexoProducerPlain producer(exporter);
    TexoImporterMarkdown importer(producer);
    FILE *file = fopen("markdown.md", "r");
    importer.Put(file);
    fclose(file);
}

void html()
{
    fputs("\n\n  ----  Markdown to HTML  ----\n\n", stdout);
    TexoExporterFile exporter(stdout);
    TexoProducerHTML producer(exporter);
    TexoImporterMarkdown importer(producer);
    FILE *file = fopen("markdown.md", "r");
    importer.Put(file);
    fclose(file);
}

void markdown()
{
    fputs("\n\n  ----  Markdown to Markdown  ----\n\n", stdout);
    TexoExporterFile exporter(stdout);
    TexoProducerMarkdown producer(exporter);
    TexoImporterMarkdown importer(producer);
    FILE *file = fopen("markdown.md", "r");
    importer.Put(file);
    fclose(file);
}

void lines()
{
    fputs("\n\n  ----  Markdown to Lines  ----\n\n", stdout);
    TexoExporterFile exporter(stdout);
    TexoProducerLines producer(exporter);
    TexoImporterMarkdown importer(producer);
    FILE *file = fopen("markdown.md", "r");
    importer.Put(file);
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