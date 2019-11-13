#include <file.hpp>
#include <html.hpp>
#include <lines.hpp>
#include <markdown.hpp>
#include <plain.hpp>
#include <stdio.h>


void plain()
{
    fputs("  ----  Plain to Plain  ----\n", stdout);
    TexoExporterFile  exporter(stdout);
    TexoProducerPlain producer(exporter);
    TexoImporterPlain importer(producer);
    FILE *            file = fopen("plain.txt", "r");
    importer.Put(file);
    importer.End();
    fclose(file);
}

void html()
{
    fputs("\n  ----  Plain to HTML  ----\n", stdout);
    TexoExporterFile  exporter(stdout);
    TexoProducerHTML  producer(exporter);
    TexoImporterPlain importer(producer);
    FILE *            file = fopen("plain.txt", "r");
    importer.Put(file);
    importer.End();
    fclose(file);
}

void markdown()
{
    fputs("\n  ----  Plain to Markdown  ----\n", stdout);
    TexoExporterFile     exporter(stdout);
    TexoProducerMarkdown producer(exporter);
    TexoImporterPlain    importer(producer);
    FILE *               file = fopen("plain.txt", "r");
    importer.Put(file);
    importer.End();
    fclose(file);
}

void lines()
{
    fputs("\n  ----  Plain to Lines  ----\n", stdout);
    TexoExporterFile  exporter(stdout);
    TexoProducerLines producer(exporter);
    TexoImporterPlain importer(producer);
    FILE *            file = fopen("plain.txt", "r");
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
