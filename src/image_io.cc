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

    pbm_header image_io::read_pbm_header( const std::string &filename )
    {
        std::ifstream in(filename, std::ios::in);
        if (!in.is_open())
           throw std::runtime_error("Could not open " + filename);

        pbm_header header;
        return header;
    }

    pgm_header image_io::read_pgm_header( const std::string &filename )
    {
        std::ifstream in(filename, std::ios::in);
        if (!in.is_open())
           throw std::runtime_error("Could not open " + filename);
        
        pgm_header header;
        return header;
    }

    ppm_header image_io::read_ppm_header( const std::string &filename )
    {
        std::ifstream in(filename, std::ios::in);
        if (!in.is_open())
           throw std::runtime_error("Could not open " + filename);
        
        ppm_header header;
        in >> header;
        return header;
    }

    pfm_header image_io::read_pfm_header( const std::string &filename )
    {
        std::ifstream in(filename, std::ios::in);
        if (!in.is_open())
           throw std::runtime_error("Could not open " + filename);
        
        pfm_header header;
        return header;
    }
}
