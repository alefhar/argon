// This file is part of libargon.
//
// libargon is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// libargon is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with libargon.  If not, see <http://www.gnu.org/licenses/>.

#ifndef AR_IMAGE_TEST_H_
#define AR_IMAGE_TEST_H_

#include "ar/image.h"
#include "ar/image_io.h"

#include <vector>
#include <gtest/gtest.h>

using image_d = argon::image<double>;

TEST (image_test, interpolate_linear_test)
{
    image_d img(2, 2);
    img(0,0) = 1; img(1,0) = 4;
    img(0,1) = 2; img(1,1) = 2;

    EXPECT_DOUBLE_EQ(2.25, img.interpolate_linear(0.5, 0.5));
    EXPECT_DOUBLE_EQ(1   , img.interpolate_linear(0., 0.));
    EXPECT_DOUBLE_EQ(4   , img.interpolate_linear(1., 0.));
    EXPECT_DOUBLE_EQ(2   , img.interpolate_linear(0., 1.));
    EXPECT_DOUBLE_EQ(2   , img.interpolate_linear(1., 1.));
}

TEST (image_test, write_as_pbm_test)
{
    argon::pbm_header header_ascii, header_binary;
    header_ascii.type   = argon::pnm_type::PBM_ASCII;
    header_ascii.magic  = static_cast<char>(header_ascii.type);
    header_ascii.width  = 5;
    header_ascii.height = 5;
   
    header_binary.type   = argon::pnm_type::PBM_BINARY; 
    header_binary.magic  = static_cast<char>(header_binary.type);
    header_binary.width  = 5;
    header_binary.height = 5;

    auto too_small = std::vector<float>(10);

    EXPECT_THROW(argon::image_io::write_as_pbm("foo.pfm", header_ascii , too_small), std::invalid_argument);
    EXPECT_THROW(argon::image_io::write_as_pbm("foo.pfm", header_binary, too_small), std::invalid_argument);
}

TEST (image_test, pbm_test)
{
    argon::image<int> image(20, 20);
    image( 9,10) = 5;
    image(10, 9) = 1;

    argon::image<int> image_clamped(20, 20);
    image_clamped( 9,10) = 1;
    image_clamped(10, 9) = 1;
 
    argon::image_io::write("image.pbm", image, argon::pnm_type::PBM_ASCII);
    argon::image_io::write("image_binary.pbm", image, argon::pnm_type::PBM_BINARY);

    auto image_in        = argon::image_io::read<int>("image.pbm");
    auto image_in_binary = argon::image_io::read<int>("image_binary.pbm");

    EXPECT_EQ(image_clamped, image_in);
    EXPECT_EQ(image_clamped, image_in_binary);
}

TEST (image_test, write_as_pgm_test)
{
    argon::pgm_header header_ascii, header_binary;
    header_ascii.type   = argon::pnm_type::PGM_ASCII;
    header_ascii.magic  = static_cast<char>(header_ascii.type);
    header_ascii.width  = 5;
    header_ascii.height = 5;
    header_ascii.bytes  = 1;
    
    header_binary.type   = argon::pnm_type::PGM_BINARY;
    header_binary.magic  = static_cast<char>(header_binary.type);
    header_binary.width  = 5;
    header_binary.height = 5;
    header_binary.bytes  = 2;

    auto too_small = std::vector<float>(10);

    EXPECT_THROW(argon::image_io::write_as_pgm("foo.pfm", header_ascii , too_small), std::invalid_argument);
    EXPECT_THROW(argon::image_io::write_as_pgm("foo.pfm", header_binary, too_small), std::invalid_argument);
}

TEST (image_test, pgm_test)
{
    argon::image<int> image(20, 20);
    argon::image<int> image_wide(20, 20);
 
    image( 9, 9) = 64;
    image(10,10) = 128;
    image( 9,10) = 16;
    image(10, 9) = 32;
 
    image_wide( 9, 9) = 16384;
    image_wide(10,10) = 32767;
    image_wide( 9,10) =  8192;
    image_wide(10, 9) =  4096;
 
    argon::image_io::write("image.pgm", image, argon::pnm_type::PGM_ASCII);
    argon::image_io::write("image_binary.pgm", image, argon::pnm_type::PGM_BINARY);
    argon::image_io::write("image_wide.pgm", image_wide, argon::pnm_type::PGM_ASCII);
    argon::image_io::write("image_wide_binary.pgm", image_wide, argon::pnm_type::PGM_BINARY);

    auto image_in             = argon::image_io::read<int>("image.pgm");
    auto image_in_binary      = argon::image_io::read<int>("image_binary.pgm");
    auto image_in_wide        = argon::image_io::read<int>("image_wide.pgm");
    auto image_in_wide_binary = argon::image_io::read<int>("image_wide_binary.pgm");

    EXPECT_EQ(image, image_in);
    EXPECT_EQ(image, image_in_binary);

    EXPECT_EQ(image_wide, image_in_wide);
    EXPECT_EQ(image_wide, image_in_wide_binary);
}

TEST (image_test, write_as_ppm_test)
{
    argon::ppm_header header_ascii, header_binary;
    header_ascii.type   = argon::pnm_type::PPM_ASCII;
    header_ascii.magic  = static_cast<char>(header_ascii.type);
    header_ascii.width  = 5;
    header_ascii.height = 5;
    header_ascii.bytes  = 1;
    
    header_binary.type   = argon::pnm_type::PPM_BINARY;
    header_binary.magic  = static_cast<char>(header_binary.type);
    header_binary.width  = 5;
    header_binary.height = 5;
    header_binary.bytes  = 2;

    auto too_small = std::vector<float>(10);

    EXPECT_THROW(argon::image_io::write_as_ppm("foo.pfm", header_ascii , too_small), std::invalid_argument);
    EXPECT_THROW(argon::image_io::write_as_ppm("foo.pfm", header_binary, too_small), std::invalid_argument);
}

TEST (image_test, ppm_test)
{
    argon::image<int> image(20, 20, 3);
    argon::image<int> image_wide(20, 20, 3);
 
    image( 9, 9,0) =  64; image( 9, 9,1) = 32; image( 9, 9,2) = 128;
    image(10,10,0) = 128; image(10,10,1) = 64; image(10,10,2) = 255;
    image( 9,10,0) =  16; image( 9,10,1) =  8; image( 9,10,2) =  32;
    image(10, 9,0) =  32; image(10, 9,1) = 16; image(10, 9,2) =  64;
    
    image_wide( 9, 9,0) = 16384; image_wide( 9, 9,1) =  8192; image_wide( 9, 9,2) = 32767;
    image_wide(10,10,0) = 32767; image_wide(10,10,1) = 16383; image_wide(10,10,2) = 65535;
    image_wide( 9,10,0) =  8192; image_wide( 9,10,1) =  4096; image_wide( 9,10,2) = 16384;
    image_wide(10, 9,0) =  4096; image_wide(10, 9,1) =  2048; image_wide(10, 9,2) =  8192;
 
    argon::image_io::write("image.ppm", image, argon::pnm_type::PPM_ASCII);
    argon::image_io::write("image_binary.ppm", image, argon::pnm_type::PPM_BINARY);
    argon::image_io::write("image_wide.ppm", image_wide, argon::pnm_type::PPM_ASCII);
    argon::image_io::write("image_wide_binary.ppm", image_wide, argon::pnm_type::PPM_BINARY);
    
    auto image_in             = argon::image_io::read<int>("image.ppm");
    auto image_in_binary      = argon::image_io::read<int>("image_binary.ppm");
    auto image_in_wide        = argon::image_io::read<int>("image_wide.ppm");
    auto image_in_wide_binary = argon::image_io::read<int>("image_wide_binary.ppm");

    EXPECT_EQ(image, image_in);
    EXPECT_EQ(image, image_in_binary);

    EXPECT_EQ(image_wide, image_in_wide);
    EXPECT_EQ(image_wide, image_in_wide_binary);
}

TEST (image_test, write_as_pfm_test)
{
    argon::pfm_header header_single, header_triple;
    header_single.type      = argon::pnm_type::PFM_SINGLE;
    header_single.magic     = static_cast<char>(header_single.type);
    header_single.width     =  5;
    header_single.height    =  5;
    header_single.endianess = -1.;
    header_single.scale     =  1.;
   
    header_triple.type      = argon::pnm_type::PFM_TRIPLE; 
    header_triple.magic     = static_cast<char>(header_triple.type);
    header_triple.width     =  5;
    header_triple.height    =  5;
    header_triple.endianess = -1.;
    header_triple.scale     =  1.;

    auto too_small = std::vector<float>(10);

    EXPECT_THROW(argon::image_io::write_as_pfm("foo.pfm", header_single, too_small), std::invalid_argument);
    EXPECT_THROW(argon::image_io::write_as_pfm("foo.pfm", header_triple, too_small), std::invalid_argument);
}

TEST (image_test, pfm_test)
{
    argon::image<float> image_single(20, 20, 1);
    argon::image<float> image_triple(20, 20, 3);

    image_triple( 9, 9,0) = 0.25f; image_triple( 9, 9,1) = 0.5f; image_triple( 9, 9,2) = 1.0f;
    image_triple(10,10,0) = 0.25f; image_triple(10,10,1) = 0.5f; image_triple(10,10,2) = 1.0f;
    image_triple( 9,10,0) = 0.25f; image_triple( 9,10,1) = 0.5f; image_triple( 9,10,2) = 1.0f;
    image_triple(10, 9,0) = 0.25f; image_triple(10, 9,1) = 0.5f; image_triple(10, 9,2) = 1.0f;
    
    image_single( 9, 9) = 0.25f;
    image_single(10,10) = 0.25f;
    image_single( 9,10) = 0.25f;
    image_single(10, 9) = 0.25f;

    argon::image_io::write("image_single.pfm", image_single, argon::pnm_type::PFM_ANY);
    argon::image_io::write("image_triple.pfm", image_triple, argon::pnm_type::PFM_ANY);

    auto image_in_single = argon::image_io::read<float>("image_single.pfm");
    auto image_in_triple = argon::image_io::read<float>("image_triple.pfm");

    EXPECT_EQ(image_single, image_in_single);
    EXPECT_EQ(image_triple, image_in_triple);
}

TEST (image_test, conversion_test)
{
    argon::image<int> image(5, 5, 1);

    auto p = 0;
    for (auto y = 0; y < image.get_height(); ++y)
    {
        for (auto x = 0; x < image.get_width(); ++x)
        {
            image(x,y) = p++;
        }
    }

    argon::image_io::write_pgm("image_conversion.pgm", image, false);
    auto image_in = argon::image_io::read_pgm<int>("image_conversion.pgm");
    EXPECT_EQ(image, image_in);

    argon::image_io::write_pfm("image_conversion.pfm", image);
    image_in = argon::image_io::read_pfm<int>("image_conversion.pfm");
    EXPECT_EQ(image, image_in);
}

#endif // AR_IMAGE_TEST_H_
