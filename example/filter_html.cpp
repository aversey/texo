#include <stdio.h>
#include <html.hpp>
#include <file.hpp>

int main()
{
    TexoFileExporter exporter(stdout);
    TexoHTMLProducer producer(exporter);
    TexoHTMLImporter importer(producer);
    importer.PutStr(
        "<script>EVIL MAGIC</script>\n"
        "<a href='#'>Some</a> br:<br/>\n"
        "<p class='evil_too'>And paragraphs work well too...</p>\n"
        "<b>You</b> <i>can</i> <del>use</del> <ins>some</ins> witchcraft.\n"
    );
    return 0;
    return 0;
}
