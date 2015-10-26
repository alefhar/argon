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

TEST (image_test, pbm_test)
{
    argon::image<int> image(20, 20);
    image( 9, 9) = 1;
    image(10,10) = 5;
    image( 9,10) = 5;
    image(10, 9) = 1;

    argon::image<int> image_clamped(20, 20);
    image_clamped( 9, 9) = 1;
    image_clamped(10,10) = 1;
    image_clamped( 9,10) = 1;
    image_clamped(10, 9) = 1;
 
    argon::image_io::write("image.pbm", image, argon::pnm_type::PBM_ASCII);
    argon::image_io::write("image_binary.pbm", image, argon::pnm_type::PBM_BINARY);

    auto image_in        = argon::image_io::read<int>("image.pbm");
    auto image_in_binary = argon::image_io::read<int>("image_binary.pbm");

    EXPECT_EQ(image_clamped, image_in);
    EXPECT_EQ(image_clamped, image_in_binary);
}

TEST (image_test, pgm_test)
{
    argon::image<int> image(20, 20);
    argon::image<int> image_wide(20, 20);
 
    image( 9, 9) = 128;
    image(10,10) = 128;
    image( 9,10) = 128;
    image(10, 9) = 128;
 
    image_wide( 9, 9) = 32767;
    image_wide(10,10) = 32767;
    image_wide( 9,10) = 32767;
    image_wide(10, 9) = 32767;
 
    argon::image_io::write("image.pgm", image, argon::pnm_type::PGM_ASCII);
    argon::image_io::write("image_binary.pgm", image, argon::pnm_type::PGM_BINARY);
    argon::image_io::write("image_wide.pgm", image_wide, argon::pnm_type::PGM_ASCII);
    argon::image_io::write("image_wide_binary.pgm", image_wide, argon::pnm_type::PGM_BINARY);

    auto image_in             = argon::image_io::read<int>("image.pgm");
    auto image_in_binary      = argon::image_io::read<int>("image_binary.pgm");
    auto image_in_wide        = argon::image_io::read<int>("image_wide.pgm");
    auto image_in_wide_binary = argon::image_io::read<int>("image_wide_binary.pgm");

    EXPECT_EQ(image, image_in);
    EXPECT_EQ(image, image_in);

    EXPECT_EQ(image_wide, image_in_wide);
    EXPECT_EQ(image_wide, image_in_wide_binary);
}

TEST (image_test, ppm_test)
{
    argon::image<int> image(20, 20, 3);
    argon::image<int> image_wide(20, 20, 3);
 
    image( 9, 9,0) = 128; image( 9, 9,1) = 64; image( 9, 9,2) = 255;
    image(10,10,0) = 128; image(10,10,1) = 64; image(10,10,2) = 255;
    image( 9,10,0) = 128; image( 9,10,1) = 64; image( 9,10,2) = 255;
    image(10, 9,0) = 128; image(10, 9,1) = 64; image(10, 9,2) = 255;
    
    image_wide( 9, 9,0) = 32767; image_wide( 9, 9,1) = 16383; image_wide( 9, 9,2) = 65535;
    image_wide(10,10,0) = 32767; image_wide(10,10,1) = 16383; image_wide(10,10,2) = 65535;
    image_wide( 9,10,0) = 32767; image_wide( 9,10,1) = 16383; image_wide( 9,10,2) = 65535;
    image_wide(10, 9,0) = 32767; image_wide(10, 9,1) = 16383; image_wide(10, 9,2) = 65535;
 
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
    std::vector<float> image_single(5 * 5);
    std::vector<float> image_triple(5 * 5 * 3);

    argon::pfm_header header_single, header_triple;
    header_single.magic = static_cast<char>(argon::pnm_type::PFM_SINGLE);
    header_single.width     =  5;
    header_single.height    =  5;
    header_single.endianess = -1.;
    header_single.scale     =  1.;
    
    header_triple.magic = static_cast<char>(argon::pnm_type::PFM_TRIPLE);
    header_triple.width     =  5;
    header_triple.height    =  5;
    header_triple.endianess = -1.;
    header_triple.scale     =  1.;

    for (auto i = 0u; i < image_single.size(); ++i)
    {
        if (i % 2 == 0)
            image_single[i] = 1.f;
        else
            image_single[i] = 0.f;
    }

    for (auto i = 0u; i < image_triple.size(); i += 3)
    {
        if (i % 2 == 0)
        {
            image_triple[i+0] = 1.f;
            image_triple[i+1] = .5f;
            image_triple[i+2] = .0f;
        }
        else
        {
            image_triple[i+0] = .0f;
            image_triple[i+1] = .0f;
            image_triple[i+2] = .0f;
        }
    }

    argon::image_io::write_as_pfm("image_single_vector.pfm", header_single, image_single);
    argon::image_io::write_as_pfm("image_triple_vector.pfm", header_triple, image_triple);

    auto image_in_single = argon::image_io::read_pfm<float>("image_single_vector.pfm");
    auto image_in_triple = argon::image_io::read_pfm<float>("image_triple_vector.pfm");

    ASSERT_EQ(5, image_in_single.get_width());
    ASSERT_EQ(5, image_in_single.get_height());
    for (auto y = 0; y < image_in_single.get_height(); ++y)
    {
        for (auto x = 0; x < image_in_single.get_width(); ++x)
        {
            if ((x + y) % 2 == 0)
                EXPECT_EQ(1.f, image_in_single(x,y));
            else
                EXPECT_EQ(0.f, image_in_single(x,y));
        }
    }

    ASSERT_EQ(5, image_in_triple.get_width());
    ASSERT_EQ(5, image_in_triple.get_height());
    for (auto y = 0; y < image_in_triple.get_height(); ++y)
    {
        for (auto x = 0; x < image_in_triple.get_width(); ++x)
        {
            if ((x + y) % 2 == 0)
            {
                EXPECT_EQ(1.f, image_in_triple(x,y,0));
                EXPECT_EQ(.5f, image_in_triple(x,y,1));
                EXPECT_EQ(.0f, image_in_triple(x,y,2));
            }
            else
            {
                EXPECT_EQ(.0f, image_in_triple(x,y,0));
                EXPECT_EQ(.0f, image_in_triple(x,y,1));
                EXPECT_EQ(.0f, image_in_triple(x,y,2));
            }
        }
    }
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

#endif // AR_IMAGE_TEST_H_
