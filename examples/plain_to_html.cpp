#include <stdio.h>
#include <html.hpp>
#include <plain.hpp>
#include <file.hpp>
#include <scrvar.hpp>


int main()
{
    TexoExporterFile exporter(stdout);
    TexoProducerHTML producer(exporter);
    TexoImporterPlain importer(producer);
    importer.Put(
        "I am a little cute line. \n"
        "I am another and that's fine. \n"
        "But I am as long as a pine,\n"
        "so lets drink a glass of wine.\n\n"
        "Second paragraph was started here. \n"
        "Take your bananas and go home, seer.\n"
    );
    return 0;
}
