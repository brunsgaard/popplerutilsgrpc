#include <iostream>

#include "poppler-document.h"
#include "poppler-page.h"
#include <poppler-document.h>
#include <poppler-image.h>
#include <poppler-page.h>
#include <poppler-page-renderer.h>


#include "popplerutilsgrpc.pb.h"

using popplerutilsgrpc::PdftoppmRequest;



static void error(const std::string &msg)
{
    std::cerr << "Error: " << msg << std::endl;
    std::cerr << "Exiting..." << std::endl;
    exit(1);
}

int render(std::string file_name)
{
    std::auto_ptr<poppler::document> doc(poppler::document::load_from_file(file_name));
    if (!doc.get()) {
        error("loading error");
    }
    
    int doc_page = 0;
    std::auto_ptr<poppler::page> p(doc->create_page(doc_page));
    if (!p.get()) {
        error("NULL page");
    }

    poppler::page_renderer pr;
    pr.set_render_hint(poppler::page_renderer::antialiasing, true);
    pr.set_render_hint(poppler::page_renderer::text_antialiasing, true);

    poppler::image img = pr.render_page(p.get(), 300, 300);
    if (!img.is_valid()) {
        error("rendering failed");
    }

    if (!img.save("outout.png", "png")) {
        error("saving to file failed");
    }
    return 0;
}

int main(int argc, char* argv[])
{
    const std::string file_name(argv[1]);
    render(file_name);
    return 0;
}
