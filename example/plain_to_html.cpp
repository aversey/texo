#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <html.hpp>
#include <plain.hpp>
#include <file.hpp>


int main()
{
    TexoFileExporter exporter(stdout);
    TexoHTMLProducer producer(exporter);
    TexoPlainImporter importer(producer);
    importer.PutStr(
        "I am a little cute line. \n"
        "I am another and that's fine. \n"
        "But I am as long as a pine,\n"
        "so lets drink a glass of wine.\n\n"
        "Second paragraph was started here. \n"
        "Take your bananas and go home, seer.\n"
    );
    return 0;
}
