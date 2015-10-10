#ifndef AR_IMAGE_TEST_H_
#define AR_IMAGE_TEST_H_

#include "ar/image.h"
#include "ar/image_io.h"

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

TEST (image_test, read_image) 
{
    auto image = argon::image_io::read_ppm<float>("image.ppm");
}

TEST (image_test, pbm_test)
{
    argon::image<int> image(20, 20);
 
    image( 9, 9) = 1;
    image(10,10) = 5;
    image( 9,10) = 5;
    image(10, 9) = 1;
 
    argon::image_io::write("image.pbm", image, argon::pnm_type::PBM_ASCII);
    argon::image_io::write("image_binary.pbm", image, argon::pnm_type::PBM_BINARY);
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
}

#endif // AR_IMAGE_TEST_H_
