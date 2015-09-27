#ifndef AR_IMAGE_UTIL_H_
#define AR_IMAGE_UTIL_H_

#include "ar/image.h"
#include "ar/pnm_types.h"

#include <string>
#include <stdexcept>

namespace argon
{
    class image_io
    {
        public:
            template <typename T>
            static image<T> read( const std::string &filename )
            {
                auto type = peek_header(filename);
                switch (type)
                {
                    case pnm_type::PBM_ASCII:
                    case pnm_type::PBM_BINARY:
                        return read_pbm<T>(filename);

                    case pnm_type::PGM_ASCII:
                    case pnm_type::PGM_BINARY:
                        return read_pgm<T>(filename);
                    
                    case pnm_type::PPM_ASCII:
                    case pnm_type::PPM_BINARY:
                        return read_ppm<T>(filename);

                    case pnm_type::PFM_SINGLE:
                    case pnm_type::PFM_TRIPLE:
                        return read_pfm<T>(filename);

                    default:
                        throw std::logic_error("Specified file is neither PBM, PGM, PPM nor PFM image");
                }
            }

            template <typename T>
            static image<T> read_pbm( const std::string &filename )
            {
                auto header = read_pbm_header(filename);
                image<T> img(header.width, header.height);

                return img; 
            }

            template <typename T>
            static image<T> read_pgm( const std::string &filename )
            {
                auto header = read_pgm_header(filename);
                image<T> img(header.width, header.height);

                return img; 
            }

            template <typename T>
            static image<T> read_ppm( const std::string &filename )
            {
                auto header = read_ppm_header(filename);
                image<T> img(header.width, header.height);

                return img; 
            }

            template <typename T>
            static image<T> read_pfm( const std::string &filename )
            {
                auto header = read_pfm_header(filename);
                int channels = 1;
                image<T> img(header.width, header.height, channels);

                return img; 
            }

            template <typename T>
            static void write( const std::string &filename, const image<T> &image, pnm_type type = pnm_type::PFM_ANY)
            {
                switch (type)
                {    
                    case pnm_type::PBM_ASCII:
                    case pnm_type::PBM_BINARY:
                        write_pbm(filename, image, type == pnm_type::PBM_BINARY);
                        break;

                    case pnm_type::PGM_ASCII:
                    case pnm_type::PGM_BINARY:
                        write_pgm(filename, image, type == pnm_type::PGM_BINARY);
                        break;
                    
                    case pnm_type::PPM_ASCII:
                    case pnm_type::PPM_BINARY:
                        write_ppm(filename, image, type == pnm_type::PPM_BINARY);
                        break;

                    default:
                    case pnm_type::PFM_ANY:
                    case pnm_type::PFM_SINGLE:
                    case pnm_type::PFM_TRIPLE:
                        write_pfm(filename, image);
                        break;
                }
            }

            template <typename T>
            static void write_pbm( const std::string &filename, const image<T> &image, bool binary = false )
            {
                auto header = get_pbm_header(image, binary);
            }

            template <typename T>
            static void write_pgm( const std::string &filename, const image<T> &image, bool binary = false )
            {
                auto header = get_pgm_header(image, binary);
            }

            template <typename T>
            static void write_ppm( const std::string &filename, const image<T> &image, bool binary = false )
            {
                auto header = get_ppm_header(image, binary);
            }

            template <typename T>
            static void write_pfm( const std::string &filename, const image<T> &image )
            {
                auto header = get_pfm_header(image);
            }
            
        private:
            static pnm_type peek_header( const std::string &filename );

            static pbm_header read_pbm_header( const std::string &filename );
            static pgm_header read_pgm_header( const std::string &filename );
            static ppm_header read_ppm_header( const std::string &filename ); 
            static pfm_header read_pfm_header( const std::string &filename );

            template <typename T>
            static pbm_header get_pbm_header( const image<T> &image, bool binary = false )
            {
                pnm_type type = binary ? pnm_type::PBM_BINARY : pnm_type::PBM_ASCII;
                
                pbm_header header{};
                header.magic  = static_cast<char>(type);
                header.width  = image.getWidth();
                header.height = image.getHeight();
                header.max    = 1;
                header.bytes  = 1;

                return header;
            }

            template <typename T>
            static pgm_header get_pgm_header( const image<T> &image, bool binary = false )
            {
                pnm_type type = binary ? pnm_type::PGM_BINARY : pnm_type::PGM_ASCII;

                pgm_header header{};
                header.magic  = static_cast<char>(type);
                header.width  = image.getWidth();
                header.height = image.getHeight();
                header.max    = 255;
                header.bytes  = header.max == 255 ? 1 : 2;

                return header;
            }

            template <typename T>
            static ppm_header get_ppm_header( const image<T> &image, bool binary = false )
            {
                pnm_type type = binary ? pnm_type::PPM_BINARY : pnm_type::PPM_ASCII;

                ppm_header header{};
                header.magic  = static_cast<char>(type);
                header.width  = image.getWidth();
                header.height = image.getHeight();
                header.max    = 255;
                header.bytes  = header.max == 255 ? 1 : 2;

                return header;
            }

            template <typename T>
            static pfm_header get_pfm_header( const image<T> &image )
            {
                pnm_type type = image.getNumChannels() == 1 ? pnm_type::PFM_SINGLE : pnm_type::PFM_TRIPLE;

                pfm_header header{};
                header.magic     = static_cast<char>(type);
                header.width     = image.getWidth();
                header.height    = image.getHeight();
                header.endianess = -1.f;
                header.scale     =  1.f;

                return header;
            }
    };
}

#endif // AR_PFM_IMAGE_H_
