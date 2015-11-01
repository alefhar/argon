#ifndef AR_IMAGE_UTIL_H_
#define AR_IMAGE_UTIL_H_

#include "ar/image.h"
#include "ar/pnm_types.h"
#include "ar/endian.h"

#include <string>
#include <vector>
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
            static std::vector<T> read_pbm_as_vector( const std::string &filename, pbm_header &header )
            {
                std::ifstream in(filename, std::ios::in | std::ios::binary);
                if (!in.is_open())
                    throw std::runtime_error(std::string("Could not open " + filename));
                
                header = read_pbm_header(in);
                std::vector<T> data(header.width * header.height);
                
                if (header.type == pnm_type::PBM_ASCII)
                {
                    int p = data.size() - header.width;
                    T val;
                    for (auto y = header.height - 1; y >= 0; --y, p -= header.width)
                    {
                        for (auto x = 0; x < header.width; ++x)
                        {
                            if (!in.good())
                                throw std::runtime_error(std::string("Error reading " + filename));
                            
                            in >> val;
                            data[p + x] = clamp8(val, header.max);
                        }
                    }

                }
                else
                { 
                    auto width = header.width;
                    int num_bits = 8;
                    int bytes_per_row = (width & 0x7) == 0 ? width / num_bits : width / num_bits + 1;
                    std::vector<std::uint8_t> binary_data((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());

                    if (binary_data.size() < static_cast<std::size_t>(header.height * bytes_per_row))
                        throw std::runtime_error(std::string("Error reading " + filename + ", not enough data"));
                     
                    int p = 0;
                    int i = data.size() - header.width;
                    for (auto y = header.height - 1; y >= 0; --y, i -= header.width)
                    {
                        auto x = 0;
                        for (int b = 0; b < bytes_per_row; ++b, ++p)
                        {
                            if (!in.good())
                                throw std::runtime_error(std::string("Error reading " + filename));

                            int mask = 0x80;
                            std::uint8_t packed = binary_data[p];
                            for (auto bit = 0; bit < num_bits && x < header.width; ++bit, ++x)
                            {
                                data[i + x] = clamp8(packed & mask, header.max);
                                mask = mask >> 1;
                            }
                        }
                    }
                }


                return data; 
            }

            template <typename T>
            static image<T> read_pbm( const std::string &filename )
            {
                pbm_header header;
                auto data = read_pbm_as_vector<T>(filename, header);
                return image<T>(header.width, header.height, 1, std::move(data));
            }

            template <typename T>
            static std::vector<T> read_pgm_as_vector( const std::string &filename, pgm_header &header )
            {
                std::ifstream in(filename, std::ios::in | std::ios::binary);
                if (!in.is_open())
                    throw std::runtime_error(std::string("Could not open " + filename));
                
                header = read_pgm_header(in);
                std::vector<T> data(header.width * header.height);

                if (header.type == pnm_type::PGM_ASCII)
                {
                    int p = data.size() - header.width;
                    T val;
                    for (auto y = header.height - 1; y >= 0; --y, p -= header.width)
                    {
                        for (auto x = 0; x < header.width; ++x)
                        {
                            if (!in.good())
                                throw std::runtime_error(std::string("Error reading " + filename));

                            in >> val;
                            if (header.bytes == 1)
                                data[p + x] = clamp8(val, header.max);
                            else
                                data[p + x] = clamp16(val, header.max);
                        }
                    }
                }
                else
                {
                    std::vector<std::uint8_t> binary_data((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
                    if (binary_data.size() < static_cast<std::size_t>(header.width * header.height * header.bytes))
                        throw std::runtime_error(std::string("Error reading " + filename + ", not enough data"));

                    auto byte_order = endianess();
                    int p = 0;
                    int i = data.size() - header.width;
                    for (auto y = header.height - 1; y >= 0; --y, i -= header.width)
                    {
                        for (auto x = 0; x < header.width; ++x, ++p)
                        {
                            if (!in.good())
                                throw std::runtime_error(std::string("Error reading " + filename));

                            if (header.bytes == 2)
                            {
                                std::uint8_t bytes[2];
                                bytes[0] = binary_data[p * 2 + 0];
                                bytes[1] = binary_data[p * 2 + 1];

                                std::uint16_t *val = reinterpret_cast<std::uint16_t *>(&bytes[0]);
                                if (byte_order == endian::LITTLE)
                                {
                                    *val = swap(*val);
                                }

                                data[i + x] = clamp16(*val, header.max);
                            }
                            else
                            {
                                data[i + x] = clamp8(binary_data[p], header.max);
                            }
                        }
                    }
                }

                return data; 
            }

            template <typename T>
            static image<T> read_pgm( const std::string &filename )
            {
                pgm_header header;
                auto data = read_pgm_as_vector<T>(filename, header);
                return image<T>(header.width, header.height, 1, std::move(data));
            }

            template <typename T>
            static std::vector<T> read_ppm_as_vector( const std::string &filename, ppm_header &header )
            {
                std::ifstream in(filename, std::ios::in | std::ios::binary);
                if (!in.is_open())
                    throw std::runtime_error(std::string("Could not open " + filename));
                
                header = read_ppm_header(in); 
                std::vector<T> data(header.width * header.height * 3);

                if (header.type == pnm_type::PPM_ASCII)
                {
                    int p = data.size() - 3 * header.width;
                    T val;
                    for (auto y = header.height - 1; y >= 0; --y, p -= 3 * header.width)
                    {
                        for (auto x = 0; x < header.width; ++x)
                        {
                            for (auto c = 0; c < 3; ++c)
                            {
                                if (!in.good())
                                    throw std::runtime_error(std::string("Error reading " + filename));

                                in >> val;
                                if (header.bytes == 2)
                                    data[p + (x * 3) + c] = clamp16(val, header.max);
                                else
                                    data[p + (x * 3) + c] = clamp8(val, header.max);
                            }
                        }
                    }
                }
                else
                {
                    std::vector<std::uint8_t> binary_data((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
                    if (binary_data.size() < static_cast<std::size_t>(header.width * header.height * 3 * header.bytes))
                        throw std::runtime_error(std::string("Error reading " + filename + ", not enough data"));

                    auto byte_order = endianess();
                    int p = 0;
                    int i = data.size() - 3 * header.width;
                    for (auto y = header.height - 1; y >= 0; --y, i -= 3 * header.width)
                    {
                        for (auto x = 0; x < header.width; ++x)
                        {
                            for (auto c = 0; c < 3; ++c, ++p)
                            {
                                if (!in.good())
                                    throw std::runtime_error(std::string("Error reading " + filename));

                                if (header.bytes == 2)
                                {
                                    std::uint8_t bytes[2];
                                    bytes[0] = binary_data[p * 2 + 0];
                                    bytes[1] = binary_data[p * 2 + 1];

                                    std::uint16_t *val = reinterpret_cast<std::uint16_t *>(&bytes[0]);
                                    if (byte_order == endian::LITTLE)
                                    {
                                        *val = swap(*val);
                                    }

                                    data[i + (x * 3) + c] = clamp16(*val, header.max);
                                }
                                else
                                {
                                    data[i + (x * 3) + c] = clamp8(binary_data[p], header.max);
                                }
                            }
                        }
                    }
                }
                
                return data;
            }

            template <typename T>
            static image<T> read_ppm( const std::string &filename )
            {
                ppm_header header;
                auto data = read_ppm_as_vector<T>(filename, header);
                return image<T>(header.width, header.height, 3, std::move(data));
            }

            template <typename T>
            static std::vector<T> read_pfm_as_vector( const std::string &filename, pfm_header &header )
            {
                std::ifstream in(filename, std::ios::in | std::ios::binary);
                if (!in.is_open())
                    throw std::runtime_error(std::string("Could not open " + filename));
                
                header = read_pfm_header(in);
                auto img_byte_order = header.endianess == 1 ? endian::BIG : endian::LITTLE;
                int channels = header.type == pnm_type::PFM_SINGLE ? 1 : 3;

                std::vector<std::uint8_t> binary_data((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
                if (binary_data.size() < static_cast<std::size_t>(header.width * header.height * channels * sizeof(float)))
                    throw std::runtime_error(std::string("Error reading " + filename + ", not enough data"));

                std::vector<T> data(header.width * header.height * channels);
                 
                auto byte_order = endianess();
                for (auto p = 0u; p < data.size(); ++p)
                {
                    if (!in.good())
                        throw std::runtime_error(std::string("Error reading " + filename));

                    std::uint8_t bytes[sizeof(float)];
                    bytes[0] = binary_data[p * sizeof(float) + 0];
                    bytes[1] = binary_data[p * sizeof(float) + 1];
                    bytes[2] = binary_data[p * sizeof(float) + 2];
                    bytes[3] = binary_data[p * sizeof(float) + 3];

                    float *val = reinterpret_cast<float *>(&bytes[0]);
                    if (byte_order != img_byte_order)
                    {
                        *val = swap(*val);
                    }

                    data[p] = *val;
                }

                return data;
            }

            template <typename T>
            static image<T> read_pfm( const std::string &filename )
            {
                pfm_header header;
                auto data = read_pfm_as_vector<T>(filename, header);
                auto channels = header.type == pnm_type::PFM_SINGLE ? 1 : 3;
                return image<T>(header.width, header.height, channels, std::move(data));
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
            static void write_as_pbm( const std::string &filename, const pbm_header &header, const std::vector<T> &data )
            {
                if (header.type != pnm_type::PBM_ASCII && header.type != pnm_type::PBM_BINARY)
                    throw std::invalid_argument("PPM header has incorrect magic byte");

                auto width    = header.width;
                auto height   = header.height;

                if (data.size() < static_cast<std::size_t>(width * height))
                    throw std::invalid_argument("'data' does not contain enough elements");

                auto binary = header.type == pnm_type::PBM_BINARY;
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
                    int p = data.size() - width;
                    for (auto y = height - 1; y >= 0; --y, p -= width)
                    {
                        for (auto x = 0; x < width; ++x)
                        {
                            out << clamp16(data[p + x], header.max) << ' ';
                        }
                        out << '\n';
                    }
                }
                else
                {
                    int num_bits = 8;
                    int bytes_per_row = (width & 0x7) == 0 ? width / num_bits : width / num_bits + 1;
                    std::vector<std::uint8_t> binary_data(bytes_per_row * width);

                    int p = data.size() - width;
                    int i = 0;
                    for (auto y = height - 1; y >= 0; --y, p -= width)
                    {
                        // Binary PBM images store each pixel as one bit,
                        // with eight pixels packed into one byte;
                        // The left-most pixel is the most significant
                        // bit in the byte.
                        auto x = 0;
                        for (auto b = 0; b < bytes_per_row; ++b, ++i)
                        {
                            auto shift = 7;
                            std::uint8_t packed = 0;
                            for (auto bit = 0; bit < 8 && x < width; ++bit, ++x)
                            {
                                packed |= clamp8(data[p + x], header.max) << shift;
                                --shift;   
                            }

                            binary_data[i] = packed;
                        }
                    }

                    std::copy(std::begin(binary_data), std::end(binary_data), std::ostreambuf_iterator<char>(out));
                }
            }

            template <typename T>
            static void write_pbm( const std::string &filename, const image<T> &image, bool binary = false )
            {
                auto header = get_pbm_header(image, binary);
                write_as_pbm(filename, header, image.data());
            }

            template <typename T>
            static void write_as_pgm( const std::string &filename, const pgm_header &header, const std::vector<T> &data )
            {
                if (header.type != pnm_type::PGM_ASCII && header.type != pnm_type::PGM_BINARY)
                    throw std::invalid_argument("PPM header has incorrect magic byte");

                auto width    = header.width;
                auto height   = header.height;

                if (data.size() < static_cast<std::size_t>(width * height))
                    throw std::invalid_argument("'data' does not contain enough elements");

                auto binary = header.type == pnm_type::PGM_BINARY;
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
                    int p = data.size() - width;
                    for (auto y = height - 1; y >= 0; --y, p -= width)
                    {
                        for (auto x = 0; x < width; ++x)
                        {
                            out << clamp16(data[p + x], header.max) << ' ';
                        }
                        out << '\n';
                    }
                }
                else
                {
                    std::vector<std::uint8_t> binary_data(width * height * header.bytes);
                    auto byte_order = endianess();

                    int p = data.size() - width;
                    int i = 0;
                    for (auto y = height - 1; y >= 0; --y, p -= width)
                    {
                        for (auto x = 0; x < width; ++x, ++i)
                        { 
                            if (header.bytes == 2)
                            {
                                std::uint16_t clamped = clamp16(data[p + x], header.max);
                                if (byte_order == endian::LITTLE)
                                    clamped = swap(clamped);

                                std::uint8_t *bytes = reinterpret_cast<std::uint8_t *>(&clamped);
                                binary_data[i * header.bytes + 0] = bytes[0];
                                binary_data[i * header.bytes + 1] = bytes[1];
                            }
                            else
                            {
                                std::uint8_t clamped = clamp8(data[p + x], header.max);
                                binary_data[i] = clamped;
                            }
                        }

                    }

                    std::copy(std::begin(binary_data), std::end(binary_data), std::ostreambuf_iterator<char>(out));
                }
            }

            template <typename T>
            static void write_pgm( const std::string &filename, const image<T> &image, bool binary = false )
            {
                auto header = get_pgm_header(image, binary);
                write_as_pgm(filename, header, image.data()); 
            }

            template <typename T>
            static void write_as_ppm( const std::string &filename, const ppm_header &header, const std::vector<T> &data )
            {
                if (header.type != pnm_type::PPM_ASCII && header.type != pnm_type::PPM_BINARY)
                    throw std::invalid_argument("PPM header has incorrect magic byte");

                auto width    = header.width;
                auto height   = header.height;
                auto channels = 3;

                if (data.size() < static_cast<std::size_t>(width * height * channels))
                    throw std::invalid_argument("'data' does not contain enough elements");

                auto binary = header.type == pnm_type::PPM_BINARY;
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
                    int p = data.size() - 3 * width;
                    for (auto y = height - 1; y >= 0; --y, p -= 3 * width)
                    {
                        for (auto x = 0; x < width; ++x)
                        {
                            for (auto c = 0; c < channels; ++c)
                            {
                                out << clamp16(data[p + (x * channels) + c], header.max) << ' ';
                            }
                        }
                        out << '\n';
                    }
                }
                else
                {
                    std::vector<std::uint8_t> binary_data(width * height * channels * header.bytes);
                    auto byte_order = endianess();

                    int p = data.size() - 3 * width;
                    int i = 0;
                    for (auto y = height - 1; y >= 0; --y, p -= 3 * width)
                    {
                        for (auto x = 0; x < width; ++x)
                        {
                            for (auto c = 0; c < channels; ++c, ++i)
                            {
                                if (header.bytes == 2)
                                {
                                    std::uint16_t clamped = clamp16(data[p + (x * channels) + c], header.max);
                                    if (byte_order == endian::LITTLE)
                                        clamped = swap(clamped);

                                    std::uint8_t* bytes = reinterpret_cast<std::uint8_t *>(&clamped);
                                    binary_data[i * header.bytes + 0] = bytes[0];
                                    binary_data[i * header.bytes + 1] = bytes[1];
                                }
                                else
                                {
                                    std::uint8_t clamped = clamp8(data[p + (x * channels) + c], header.max);
                                    binary_data[i] = clamped;
                                }
                            }
                        }
                    }

                    // for (auto p = 0u; p < data.size(); ++p)
                    // {
                    //     if (header.bytes == 2)
                    //     {
                    //         std::uint16_t clamped = clamp16(data[p], header.max);
                    //         if (byte_order == endian::LITTLE)
                    //             clamped = swap(clamped);

                    //         std::uint8_t* bytes = reinterpret_cast<std::uint8_t *>(&clamped);
                    //         binary_data[p * header.bytes + 0] = bytes[0];
                    //         binary_data[p * header.bytes + 1] = bytes[1];
                    //     }
                    //     else
                    //     {
                    //         std::uint8_t clamped = clamp8(data[p], header.max);
                    //         binary_data[p] = clamped;
                    //     }
                    // }

                    std::copy(std::begin(binary_data), std::end(binary_data), std::ostreambuf_iterator<char>(out));
                }
            }

            template <typename T>
            static void write_ppm( const std::string &filename, const image<T> &image, bool binary = false )
            {
                if (image.get_num_channels() != 3)
                    throw std::invalid_argument("Image must have exactly three channels");

                auto header = get_ppm_header(image, binary);
                write_as_ppm(filename, header, image.data());
            }

            template <typename T>
            static void write_as_pfm( const std::string &filename, const pfm_header &header, const std::vector<T> &data )
            {
                if (header.type != pnm_type::PFM_SINGLE && header.type != pnm_type::PFM_TRIPLE)
                    throw std::invalid_argument("PFM header has incorrect magic byte");

                auto width    = header.width;
                auto height   = header.height;
                auto channels = header.type == pnm_type::PFM_SINGLE ? 1 : 3;

                if (data.size() < static_cast<std::size_t>(width * height * channels))
                    throw std::invalid_argument("'data' does not contain enough elements");

                std::vector<std::uint8_t> binary_data(width * height * channels * sizeof(float));
                auto byte_order = endianess();
                
                for (auto p = 0u; p < data.size(); ++p)
                {
                    float pixel = data[p];
                    if (byte_order == endian::BIG)
                        pixel = swap(pixel);

                    std::uint8_t *bytes = reinterpret_cast<std::uint8_t *>(&pixel);
                    binary_data[p * sizeof(float) + 0] = bytes[0];
                    binary_data[p * sizeof(float) + 1] = bytes[1];
                    binary_data[p * sizeof(float) + 2] = bytes[2];
                    binary_data[p * sizeof(float) + 3] = bytes[3];

                }
                
                auto mode = std::ios::out | std::ios::binary;
                std::ofstream out(filename, mode);
                if (!out.is_open())
                    throw std::runtime_error(std::string("Could not open " + filename));

                out << header;
                std::copy(std::begin(binary_data), std::end(binary_data), std::ostreambuf_iterator<char>(out));
            }

            template <typename T>
            static void write_pfm( const std::string &filename, const image<T> &image )
            {
                if (image.get_num_channels() != 3 && image.get_num_channels() != 1)
                    throw std::invalid_argument("Image must have exactly one or three channels");

                auto header = get_pfm_header(image);
                write_as_pfm(filename, header, image.data());
            }
            
            template <typename T>
            static pbm_header get_pbm_header( const image<T> &image, bool binary = false )
            {
                pbm_header header{};
                header.type   = binary ? pnm_type::PBM_BINARY : pnm_type::PBM_ASCII;
                header.magic  = static_cast<char>(header.type);
                header.width  = image.get_width();
                header.height = image.get_height();
                header.max    = 1;

                return header;
            }

            template <typename T>
            static pgm_header get_pgm_header( const image<T> &image, bool binary = false )
            {
                pgm_header header{};
                header.type   = binary ? pnm_type::PGM_BINARY : pnm_type::PGM_ASCII;
                header.magic  = static_cast<char>(header.type);
                header.width  = image.get_width();
                header.height = image.get_height();
                header.max    = image.max_element() <= 255 ? 255 : 65535;
                header.bytes  = header.max == 255 ? 1 : 2;

                return header;
            }

            template <typename T>
            static ppm_header get_ppm_header( const image<T> &image, bool binary = false )
            {
                ppm_header header{};
                header.type   = binary ? pnm_type::PPM_BINARY : pnm_type::PPM_ASCII;
                header.magic  = static_cast<char>(header.type);
                header.width  = image.get_width();
                header.height = image.get_height();
                header.max    = image.max_element() <= 255 ? 255 : 65535;
                header.bytes  = header.max == 255 ? 1 : 2;

                return header;
            }

            template <typename T>
            static pfm_header get_pfm_header( const image<T> &image )
            {
                pfm_header header{};
                header.type      = image.get_num_channels() == 1 ? pnm_type::PFM_SINGLE : pnm_type::PFM_TRIPLE;
                header.magic     = static_cast<char>(header.type);
                header.width     = image.get_width();
                header.height    = image.get_height();
                header.endianess = -1.f;
                header.scale     =  1.f;

                return header;
            }
        
        private:
            static pnm_type peek_header( const std::string &filename );

            static pbm_header read_pbm_header( std::ifstream &in );
            static pgm_header read_pgm_header( std::ifstream &in );
            static ppm_header read_ppm_header( std::ifstream &in ); 
            static pfm_header read_pfm_header( std::ifstream &in );

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
