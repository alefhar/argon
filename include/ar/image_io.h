#ifndef AR_IMAGE_UTIL_H_
#define AR_IMAGE_UTIL_H_

#include "ar/image.h"

#include <string>
#include <stdexcept>

namespace argon
{
    enum class pnm_type : char
    { 
        PBM_ASCII  = '1', PGM_ASCII  = '2', PPM_ASCII  = '3',
        PBM_BINARY = '4', PGM_BINARY = '5', PPM_BINARY = '6',
        PFM_SINGLE = 'f', PFM_TRIPLE  = 'F'
    };

    struct header_data
    {
        char magic;
        int  width;
        int  height;
    };

    struct pnm_header : header_data
    {
        int max;
    };

    struct pbm_header : pnm_header
    {
        const int allowed_max = 1;
    };

    struct pgm_header : pnm_header
    {
        const int allowed_max = 65535;
    };

    struct ppm_header : pnm_header
    {
        const int allowed_max = 65535;
    };

    struct pfm_header : header_data
    {
        float endianess;
        float scale;
    };

    using pbm_image = image<char>;
    using pgm_image = image<int>;
    using ppm_image = image<int>;
    using pfm_image = image<float>;
    
    class image_io
    {
        public:
            template<typename T> static image<T> read( const std::string& filename )
            {
                auto type = peek_header(filename);
                
                switch (type)
                {
                    case pnm_type::PBM_ASCII:
                    case pnm_type::PBM_BINARY:
                        return read_pbm(filename);
                        break;

                    case pnm_type::PGM_ASCII:
                    case pnm_type::PGM_BINARY:
                        return read_pgm(filename);
                        break;
                    
                    case pnm_type::PPM_ASCII:
                    case pnm_type::PPM_BINARY:
                        return read_ppm(filename);
                        break;

                    case pnm_type::PFM_SINGLE:
                    case pnm_type::PFM_TRIPLE:
                        return read_pfm(filename);
                        break;

                    default:
                        throw new std::logic_error("File '" + filename + "' is neither a PBM, PGM, PPM or PFM image");
                }
            }

            static pbm_image read_pbm( const std::string &filename );
            static pgm_image read_pgm( const std::string &filename );
            static ppm_image read_ppm( const std::string &filename );
            static pfm_image read_pfm( const std::string &filename );
            
        private:
            static pnm_type peek_header( const std::string &filename );

            static pbm_header read_pbm_header( const std::string &filename );
            static pgm_header read_pgm_header( const std::string &filename );
            static ppm_header read_ppm_header( const std::string &filename ); 
            static pfm_header read_pfm_header( const std::string &filename );
    };
}

#endif // AR_PFM_IMAGE_H_
