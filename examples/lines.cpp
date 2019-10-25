#include <stdio.h>
#include <html.hpp>
#include <plain.hpp>
#include <lines.hpp>
#include <markdown.hpp>
#include <file.hpp>


void plain()
{
    fputs("  ----  Lines to Plain  ----\n\n", stdout);
    TexoExporterFile exporter(stdout);
    TexoProducerPlain producer(exporter);
    TexoImporterLines importer(producer);
    FILE *file = fopen("lines.txt", "r");
    importer.Put(file);
    fclose(file);
}

void html()
{
    fputs("\n\n  ----  Lines to HTML  ----\n\n", stdout);
    TexoExporterFile exporter(stdout);
    TexoProducerHTML producer(exporter);
    TexoImporterLines importer(producer);
    FILE *file = fopen("lines.txt", "r");
    importer.Put(file);
    fclose(file);
}

void markdown()
{
    fputs("\n\n  ----  Lines to Markdown  ----\n\n", stdout);
    TexoExporterFile exporter(stdout);
    TexoProducerMarkdown producer(exporter);
    TexoImporterLines importer(producer);
    FILE *file = fopen("lines.txt", "r");
    importer.Put(file);
    fclose(file);
}

void lines()
{
    fputs("\n\n  ----  Lines to Lines  ----\n\n", stdout);
    TexoExporterFile exporter(stdout);
    TexoProducerLines producer(exporter);
    TexoImporterLines importer(producer);
    FILE *file = fopen("lines.txt", "r");
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
