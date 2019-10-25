#include <stdio.h>
#include <html.hpp>
#include <plain.hpp>
#include <lines.hpp>
#include <markdown.hpp>
#include <file.hpp>


void plain()
{
    fputs("  ----  Plain to Plain  ----\n\n", stdout);
    TexoExporterFile exporter(stdout);
    TexoProducerPlain producer(exporter);
    TexoImporterPlain importer(producer);
    FILE *file = fopen("plain.txt", "r");
    importer.Put(file);
    fclose(file);
}

void html()
{
    fputs("\n\n  ----  Plain to HTML  ----\n\n", stdout);
    TexoExporterFile exporter(stdout);
    TexoProducerHTML producer(exporter);
    TexoImporterPlain importer(producer);
    FILE *file = fopen("plain.txt", "r");
    importer.Put(file);
    fclose(file);
}

void markdown()
{
    fputs("\n\n  ----  Plain to Markdown  ----\n\n", stdout);
    TexoExporterFile exporter(stdout);
    TexoProducerMarkdown producer(exporter);
    TexoImporterPlain importer(producer);
    FILE *file = fopen("plain.txt", "r");
    importer.Put(file);
    fclose(file);
}

void lines()
{
    fputs("\n\n  ----  Plain to Lines  ----\n\n", stdout);
    TexoExporterFile exporter(stdout);
    TexoProducerLines producer(exporter);
    TexoImporterPlain importer(producer);
    FILE *file = fopen("plain.txt", "r");
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
