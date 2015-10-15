#ifndef AR_IMAGE_UTIL_H_
#define AR_IMAGE_UTIL_H_

#include "ar/image.h"
#include "ar/pnm_types.h"
#include "ar/endian.h"

#include <string>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <bitset>

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

                auto type = static_cast<pnm_type>(header.magic);
                auto mode = std::ios::in;
                if (type == pnm_type::PBM_BINARY)
                {
                    mode |= std::ios::binary;
                }
                
                std::ifstream in(filename, mode);
                if (!in.is_open())
                    throw std::runtime_error(std::string("Could not open " + filename));
                
                image<T> img(header.width, header.height);
                if (type == pnm_type::PBM_ASCII)
                {
                    T val;
                    for (int y = 0; y < img.get_height(); ++y)
                    {
                        for (int x = 0; x < img.get_width(); ++x)
                        {
                            if (!in.good())
                                throw std::runtime_error(std::string("Error reading image " + filename));
                            
                            in >> val;       
                            img(x,y) = clamp8(val, header.max);
                        }
                    }

                }
                else
                {

                }

                return img; 
            }

            template <typename T>
            static image<T> read_pgm( const std::string &filename )
            {
                auto header = read_pgm_header(filename);

                auto type = static_cast<pnm_type>(header.magic);
                auto mode = std::ios::in;
                if (type == pnm_type::PGM_BINARY)
                {
                    mode |= std::ios::binary;
                }
                
                std::ifstream in(filename, mode);
                if (!in.is_open())
                    throw std::runtime_error(std::string("Could not open " + filename));
                
                image<T> img(header.width, header.height);
                if (type == pnm_type::PGM_ASCII)
                {
                    T val;
                    for (int y = 0; y < img.get_height(); ++y)
                    {
                        for (int x = 0; x < img.get_width(); ++x)
                        {
                            if (!in.good())
                                throw std::runtime_error(std::string("Error reading image " + filename));
                            
                            in >> val;
                            if (header.bytes == 1)
                                img(x,y) = clamp8(val, header.max);
                            else
                                img(x,y) = clamp16(val, header.max);
                        }
                    }
                }
                else
                {

                }

                return img; 
            }

            template <typename T>
            static image<T> read_ppm( const std::string &filename )
            {
                auto header = read_ppm_header(filename);
                
                auto type = static_cast<pnm_type>(header.magic);
                auto mode = std::ios::in;
                if (type == pnm_type::PPM_BINARY)
                {
                    mode |= std::ios::binary;
                }
                
                std::ifstream in(filename, mode);
                if (!in.is_open())
                    throw std::runtime_error(std::string("Could not open " + filename));
                
                image<T> img(header.width, header.height, 3);
                if (type == pnm_type::PPM_ASCII)
                {

                }
                else
                {

                }

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

                auto mode = std::ios::out;
                if (binary)
                {
                    mode |= std::ios::binary;
                }

                std::ofstream out(filename, mode);
                if (!out.is_open())
                    throw std::runtime_error(std::string("Could not open " + filename));

                out << header;

                if (!binary)
                {
                    for (int y = 0; y < image.get_height(); ++y)
                    {
                        for (int x = 0; x < image.get_width(); ++x)
                        {
                            out << clamp8(image(x,y), header.max) << ' ';
                        }
                        out << '\n';
                    }
                }
                else
                {
                    for (int y = 0; y < image.get_height(); ++y)
                    {
                        // Binary PBM images store each pixel as one bit,
                        // with eight pixels packed into one byte;
                        // The left-most pixel is the most significant
                        // bit in the byte.
                        int written = 0;
                        for (int x = 0; x < image.get_width() - 7; x += 8)
                        {
                            int shift = 7;
                            std::uint8_t packed = 0;
                            for (int b = 0; b < 8; ++b)
                            {
                                packed |= clamp8(image(x+b,y), header.max) << shift;
                                --shift;
                            }
                            out << packed;
                            written += 8;
                        }

                        // If length isn't divisible by eight, pack
                        // remaining pixels into a last byte and fill
                        // with zeros.
                        int remaining = image.get_width() - written;
                        if (remaining > 0)
                        {
                            int shift = 7;
                            std::uint8_t packed = 0;
                            for (int b = 0; b < remaining; ++b)
                            {
                                packed |= clamp8(image(written+b,y), header.max) << shift;
                                --shift;
                            }
                            out << packed;
                        }
                    }
                }
            }

            template <typename T>
            static void write_pgm( const std::string &filename, const image<T> &image, bool binary = false )
            {
                auto header = get_pgm_header(image, binary);
                
                auto mode = std::ios::out;
                if (binary)
                {
                    mode |= std::ios::binary;
                }

                std::ofstream out(filename, mode);
                if (!out.is_open())
                    throw std::runtime_error(std::string("Could not open " + filename));

                out << header;
                
                if (!binary)
                {
                    for (int y = 0; y < image.get_height(); ++y)
                    {
                        for (int x = 0; x < image.get_width(); ++x)
                        {
                            out << clamp16(image(x,y), header.max) << ' ';
                        }
                        out << '\n';
                    }
                }
                else
                {
                    if (header.bytes == 2)
                    {
                        auto byte_order = endianess();
                        
                        for (int y = 0; y < image.get_height(); ++y)
                        {
                            for (int x = 0; x < image.get_width(); ++x)
                            {
                                std::uint16_t clamped = clamp16(image(x,y), header.max);
                                if (byte_order == endian::LITTLE)
                                    clamped = swap(clamped);

                                char *bytes = reinterpret_cast<char *>(&clamped);
                                out << bytes[0] << bytes[1];
                            }
                        }
                    }
                    else
                    {
                        for (int y = 0; y < image.get_height(); ++y)
                        {
                            for (int x = 0; x < image.get_width(); ++x)
                            {
                                std::uint8_t clamped = clamp8(image(x,y), header.max);
                                out << clamped;
                            }
                        }
                    }
                }
            }

            template <typename T>
            static void write_ppm( const std::string &filename, const image<T> &image, bool binary = false )
            {
                if (image.get_num_channels() != 3)
                    throw std::invalid_argument("Image must have exactly three channels");

                auto header = get_ppm_header(image, binary);
                
                auto mode = std::ios::out;
                if (binary)
                {
                    mode |= std::ios::binary;
                }

                std::ofstream out(filename, mode);
                if (!out.is_open())
                    throw std::runtime_error(std::string("Could not open " + filename));

                out << header;
                
                if (!binary)
                {
                    for (int y = 0; y < image.get_height(); ++y)
                    {
                        for (int x = 0; x < image.get_width(); ++x)
                        {
                            for (int c = 0; c < 3; ++c)
                            {
                                out << clamp16(image(x,y,c), header.max) << ' ';
                            }
                        }
                        out << '\n';
                    }
                }
                else
                {
                    if (header.bytes == 2)
                    {
                        auto byte_order = endianess();

                        for (int y = 0; y < image.get_height(); ++y)
                        {
                            for (int x = 0; x < image.get_width(); ++x)
                            {
                                for (int c = 0; c < image.get_num_channels(); ++c)
                                {
                                    std::uint16_t clamped = clamp16(image(x,y,c), header.max);
                                    if (byte_order == endian::LITTLE)
                                        clamped = swap(clamped);

                                    char *bytes = reinterpret_cast<char *>(&clamped);
                                    out << bytes[0] << bytes[1];
                                }
                            }
                        }
                    }
                    else
                    {
                        for (int y = 0; y < image.get_height(); ++y)
                        {
                            for (int x = 0; x < image.get_width(); ++x)
                            {
                                for (int c = 0; c < image.get_num_channels(); ++c)
                                {
                                    std::uint8_t clamped = clamp8(image(x,y,c), header.max);
                                    out << clamped;
                                }
                            }
                        }
                    }
                }
            }

            template <typename T>
            static void write_pfm( const std::string &filename, const image<T> &image )
            {
                if (image.get_num_channels() != 3 && image.get_num_channels() != 1)
                    throw std::invalid_argument("Image must have exactly one or three channels");

                auto header = get_pfm_header(image);
                
                auto mode = std::ios::out | std::ios::binary;
                std::ofstream out(filename, mode);
                if (!out.is_open())
                    throw std::runtime_error(std::string("Could not open " + filename));

                out << header;

                auto byte_order = endianess();

                for (int y = 0; y < image.get_height(); ++y)
                {
                    for (int x = 0; x < image.get_width(); ++x)
                    {
                        for (int c = 0; c < image.get_num_channels(); ++c)
                        {
                            float pixel = image(x,y,c);
                            if (byte_order == endian::BIG)
                                pixel = swap(pixel);

                            char * bytes = reinterpret_cast<char*>(&pixel);
                            out << bytes[0] << bytes[1] << bytes[2] << bytes[3];
                        }
                    }
                }
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
                header.width  = image.get_width();
                header.height = image.get_height();

                return header;
            }

            template <typename T>
            static pgm_header get_pgm_header( const image<T> &image, bool binary = false )
            {
                pnm_type type = binary ? pnm_type::PGM_BINARY : pnm_type::PGM_ASCII;

                pgm_header header{};
                header.magic  = static_cast<char>(type);
                header.width  = image.get_width();
                header.height = image.get_height();
                header.max    = image.max_element() <= 255 ? 255 : 65535;
                header.bytes  = header.max == 255 ? 1 : 2;

                return header;
            }

            template <typename T>
            static ppm_header get_ppm_header( const image<T> &image, bool binary = false )
            {
                pnm_type type = binary ? pnm_type::PPM_BINARY : pnm_type::PPM_ASCII;

                ppm_header header{};
                header.magic  = static_cast<char>(type);
                header.width  = image.get_width();
                header.height = image.get_height();
                header.max    = image.max_element() <= 255 ? 255 : 65535;
                header.bytes  = header.max == 255 ? 1 : 2;

                return header;
            }

            template <typename T>
            static pfm_header get_pfm_header( const image<T> &image )
            {
                pnm_type type = image.get_num_channels() == 1 ? pnm_type::PFM_SINGLE : pnm_type::PFM_TRIPLE;

                pfm_header header{};
                header.magic     = static_cast<char>(type);
                header.width     = image.get_width();
                header.height    = image.get_height();
                header.endianess = -1.f;
                header.scale     =  1.f;

                return header;
            }

            template <typename T, typename M>
            static std::uint8_t clamp8( T val, M max )
            {
                T max_val = static_cast<T>(max);
                return static_cast<std::uint8_t>(std::max(T{0}, std::min(val, max_val)));
            }

            template <typename T, typename M>
            static std::uint16_t clamp16( T val, M max )
            {
                T max_val = static_cast<T>(max);
                return static_cast<std::uint16_t>(std::max(T{0}, std::min(val, max_val)));
            }
    };
}

#endif // AR_PFM_IMAGE_H_
