#include <stdio.h>
#include <html.hpp>
#include <plain.hpp>
#include <lines.hpp>
#include <markdown.hpp>
#include <file.hpp>


void plain()
{
    fputs("  ----  Markdown to Plain  ----\n", stdout);
    TexoExporterFile exporter(stdout);
    TexoProducerPlain producer(exporter);
    TexoImporterMarkdown importer(producer);
    FILE *file = fopen("markdown.md", "r");
    importer.Put(file);
    importer.End();
    fclose(file);
}

void html()
{
    fputs("\n  ----  Markdown to HTML  ----\n", stdout);
    TexoExporterFile exporter(stdout);
    TexoProducerHTML producer(exporter);
    TexoImporterMarkdown importer(producer);
    FILE *file = fopen("markdown.md", "r");
    importer.Put(file);
    importer.End();
    fclose(file);
}

void markdown()
{
    fputs("\n  ----  Markdown to Markdown  ----\n", stdout);
    TexoExporterFile exporter(stdout);
    TexoProducerMarkdown producer(exporter);
    TexoImporterMarkdown importer(producer);
    FILE *file = fopen("markdown.md", "r");
    importer.Put(file);
    importer.End();
    fclose(file);
}

void lines()
{
    fputs("\n  ----  Markdown to Lines  ----\n", stdout);
    TexoExporterFile exporter(stdout);
    TexoProducerLines producer(exporter);
    TexoImporterMarkdown importer(producer);
    FILE *file = fopen("markdown.md", "r");
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
