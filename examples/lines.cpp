#include <stdio.h>
#include <html.hpp>
#include <plain.hpp>
#include <lines.hpp>
#include <markdown.hpp>
#include <file.hpp>


void plain()
{
    fputs("  ----  Lines to Plain  ----\n", stdout);
    TexoExporterFile exporter(stdout);
    TexoProducerPlain producer(exporter);
    TexoImporterLines importer(producer);
    FILE *file = fopen("lines.txt", "r");
    importer.Put(file);
    importer.End();
    fclose(file);
}

void html()
{
    fputs("\n  ----  Lines to HTML  ----\n", stdout);
    TexoExporterFile exporter(stdout);
    TexoProducerHTML producer(exporter);
    TexoImporterLines importer(producer);
    FILE *file = fopen("lines.txt", "r");
    importer.Put(file);
    importer.End();
    fclose(file);
}

void markdown()
{
    fputs("\n  ----  Lines to Markdown  ----\n", stdout);
    TexoExporterFile exporter(stdout);
    TexoProducerMarkdown producer(exporter);
    TexoImporterLines importer(producer);
    FILE *file = fopen("lines.txt", "r");
    importer.Put(file);
    importer.End();
    fclose(file);
}

void lines()
{
    fputs("\n  ----  Lines to Lines  ----\n", stdout);
    TexoExporterFile exporter(stdout);
    TexoProducerLines producer(exporter);
    TexoImporterLines importer(producer);
    FILE *file = fopen("lines.txt", "r");
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
