#include "ar/image_io.h"
#include "ar/image.h"

#include <fstream>
#include <iostream>

namespace argon
{
    pbm_header image_io::read_pbm_header( const std::string &filename )
    {
        pbm_header header;

        return header;
    }

    pgm_header image_io::read_pgm_header( const std::string &filename )
    {
        pgm_header header;

        return header;
    }

    ppm_header image_io::read_ppm_header( const std::string &filename )
    {
        ppm_header header;

        return header;
    }

    pfm_header image_io::read_pfm_header( const std::string &filename )
    {
        pfm_header header;
        
        return header;
    }
}
