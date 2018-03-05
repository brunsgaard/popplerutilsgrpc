#include <iostream>
#include <fstream>
#include <string>


#include "goo/PNGWriter.h"

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

int render_proto()
{
    std::cout  << "Output sentence";
    return 1;
}


int main(int argc, char* argv[])
{
    const std::string filename (argv[1]);

    std::ifstream ifs (filename, std::ios::binary|std::ios::ate);
    std::ifstream::pos_type pos = ifs.tellg();

    //std::vector<char> result (pos);
    poppler::byte_array result (pos); 

    ifs.seekg(0, std::ios::beg);
    ifs.read(&result[0], pos);
    std::auto_ptr<poppler::document> doc(poppler::document::load_from_data(&result));
    int doc_page = 0;
    std::auto_ptr<poppler::page> p(doc->create_page(doc_page));
    poppler::page_renderer pr;
    pr.set_render_hint(poppler::page_renderer::antialiasing, true);
    pr.set_render_hint(poppler::page_renderer::text_antialiasing, true);

    poppler::image img = pr.render_page(p.get(), 300, 300);

    std::string fmt = "png";
    const int actual_dpi = 75;

    std::unique_ptr<ImgWriter> w;
    //w.reset(new PNGWriter());



    return 0;
}
