#include "ar/image_io.h"
#include "ar/image.h"

#include <fstream>
#include <iostream>

namespace argon
{
    pbm_image image_io::read_pbm( const std::string &filename )
    {
        auto header = read_pbm_header(filename);
        pbm_image img(header.width, header.height);

        return img; 
    }
    
    pgm_image image_io::read_pgm( const std::string &filename )
    {
        auto header = read_pgm_header(filename);
        pgm_image img(header.width, header.height);

        return img; 
    }
    
    ppm_image image_io::read_ppm( const std::string &filename )
    {
        auto header = read_ppm_header(filename);
        ppm_image img(header.width, header.height);

        return img; 
    }
    
    pfm_image image_io::read_pfm( const std::string &filename )
    {
        auto header = read_pfm_header(filename);
        int channels = 1;
        pfm_image img(header.width, header.height, channels);

        return img; 
    }

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
