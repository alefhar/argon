#ifndef AR_IMAGE_UTIL_H_
#define AR_IMAGE_UTIL_H_

#include "ar/image.h"
#include "ar/pnm_types.h"
#include "ar/endian.h"

#include <string>
#include <iostream>
#include <fstream>
#include <iterator>
#include <cmath>
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
                    T val;
                    for (int y = 0; y < img.get_height(); ++y)
                    {
                        for (int x = 0; x < img.get_width(); ++x)
                        {
                            for (int c = 0; c < img.get_num_channels(); ++c)
                            {
                                if (!in.good())
                                    throw std::runtime_error(std::string("Error reading image " + filename));

                                in >> val;
                                if (header.bytes == 1)
                                    img(x,y,c) = clamp8(val, header.max);
                                else
                                    img(x,y,c) = clamp16(val, header.max);
                            }
                        }
                    }
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

                auto width  = image.get_width();
                auto height = image.get_height();

                if (!binary)
                {
                    for (int y = 0; y < height; ++y)
                    {
                        for (int x = 0; x < width; ++x)
                        {
                            out << clamp16(image(x,y), header.max) << ' ';
                        }
                        out << '\n';
                    }
                }
                else
                {
                    int num_bits = 8;
                    int bytes_per_row = (width & 0x7) == 0 ? width / num_bits : width / num_bits + 1;
                    std::vector<std::uint8_t> binary_data(bytes_per_row * width);

                    int ptr = 0;
                    for (int y = 0; y < height; ++y)
                    {
                        // Binary PBM images store each pixel as one bit,
                        // with eight pixels packed into one byte;
                        // The left-most pixel is the most significant
                        // bit in the byte.
                        int x = 0;
                        for (int b = 0; b < bytes_per_row; ++b)
                        {
                            int shift = 7;
                            std::uint8_t packed = 0;
                            for (int i = 0; i < 8 && x < width; ++i, ++x)
                            {
                                packed |= clamp8(image(x,y), header.max) << shift;
                                --shift;   
                            }

                            binary_data[ptr] = packed;
                            ++ptr;
                        }
                    }

                    std::copy(std::begin(binary_data), std::end(binary_data), std::ostreambuf_iterator<char>(out));
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

                auto width  = image.get_width();
                auto height = image.get_height();
                
                if (!binary)
                {
                    for (int y = 0; y < height; ++y)
                    {
                        for (int x = 0; x < width; ++x)
                        {
                            out << clamp16(image(x,y), header.max) << ' ';
                        }
                        out << '\n';
                    }
                }
                else
                {
                    std::vector<std::uint8_t> binary_data(width * height * header.bytes);
                    auto byte_order = endianess();
                    
                    int ptr = 0;                    
                    for (int y = 0; y < height; ++y)
                    {
                        for (int x = 0; x < width; ++x)
                        {
                            if (header.bytes == 2)
                            {
                                std::uint16_t clamped = clamp16(image(x,y), header.max);
                                if (byte_order == endian::LITTLE)
                                    clamped = swap(clamped);

                                std::uint8_t *bytes = reinterpret_cast<std::uint8_t *>(&clamped);
                                binary_data[ptr * header.bytes + 0] = bytes[0];
                                binary_data[ptr * header.bytes + 1] = bytes[1];
                            }
                            else
                            {
                                std::uint8_t clamped = clamp8(image(x,y), header.max);
                                binary_data[ptr] = clamped;
                            }

                            ++ptr;
                        }
                    }

                    std::copy(std::begin(binary_data), std::end(binary_data), std::ostreambuf_iterator<char>(out));
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
                
                auto width    = image.get_width();
                auto height   = image.get_height();
                auto channels = image.get_num_channels();
                
                if (!binary)
                {
                    for (int y = 0; y < height; ++y)
                    {
                        for (int x = 0; x < width; ++x)
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
                    std::vector<std::uint8_t> binary_data(width * height * channels * header.bytes);
                    auto byte_order = endianess();

                    int ptr = 0;
                    for (int y = 0; y < height; ++y)
                    {
                        for (int x = 0; x < width; ++x)
                        {
                            for (int c = 0; c < channels; ++c)
                            {
                                if (header.bytes == 2)
                                {
                                    std::uint16_t clamped = clamp16(image(x,y,c), header.max);
                                    if (byte_order == endian::LITTLE)
                                        clamped = swap(clamped);
                                    
                                    std::uint8_t* bytes = reinterpret_cast<std::uint8_t *>(&clamped);
                                    binary_data[ptr * header.bytes + 0] = bytes[0];
                                    binary_data[ptr * header.bytes + 1] = bytes[1];
                                }
                                else
                                {
                                    std::uint8_t clamped = clamp8(image(x,y,c), header.max);
                                    binary_data[ptr] = clamped;
                                }

                                ++ptr;
                            }
                        }
                    }

                    std::copy(std::begin(binary_data), std::end(binary_data), std::ostreambuf_iterator<char>(out));
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

                auto width    = image.get_width();
                auto height   = image.get_height();
                auto channels = image.get_num_channels();
                
                std::vector<std::uint8_t> binary_data(width * height * channels * sizeof(float));
                auto byte_order = endianess();

                int ptr = 0;
                for (int y = 0; y < height; ++y)
                {
                    for (int x = 0; x < width; ++x)
                    {
                        for (int c = 0; c < channels; ++c)
                        {
                            float pixel = image(x,y,c);
                            if (byte_order == endian::BIG)
                                pixel = swap(pixel);

                            std::uint8_t *bytes = reinterpret_cast<std::uint8_t *>(&pixel);
                            binary_data[ptr * sizeof(float) + 0] = bytes[0];
                            binary_data[ptr * sizeof(float) + 1] = bytes[1];
                            binary_data[ptr * sizeof(float) + 2] = bytes[2];
                            binary_data[ptr * sizeof(float) + 3] = bytes[3];

                            ++ptr;
                        }
                    }
                }

                std::copy(std::begin(binary_data), std::end(binary_data), std::ostreambuf_iterator<char>(out));
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
                header.max    = 1;

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
