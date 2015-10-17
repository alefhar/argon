#include "ar/image_io.h"
#include "ar/image.h"

#include <fstream>
#include <iostream>

namespace argon
{
    pnm_type image_io::peek_header( const std::string &filename )
    {
        std::ifstream in(filename, std::ios::in);
        if (!in.is_open())
           throw std::runtime_error("Could not open " + filename);

        char magic[2];
        in.read(&magic[0], 2);

        return static_cast<pnm_type>(magic[1]);
    }

    pbm_header image_io::read_pbm_header( std::ifstream &in )
    {
        pbm_header header;
        in >> header;
        return header;
    }

    pgm_header image_io::read_pgm_header( std::ifstream &in )
    {
        pgm_header header;
        in >> header;
        return header;
    }

    ppm_header image_io::read_ppm_header( std::ifstream &in )
    {
        ppm_header header;
        in >> header;
        return header;
    }

    pfm_header image_io::read_pfm_header( std::ifstream &in )
    {
        pfm_header header;
        in >> header;
        return header;
    }
}
