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

TEST (image_test, write_image)
{
    argon::image<int> pbm_image(20, 20);
    pbm_image( 9, 9) = 1;
    pbm_image(10,10) = 5;
    pbm_image( 9,10) = 5;
    pbm_image(10, 9) = 1;
    argon::image_io::write("image.pbm", pbm_image, argon::pnm_type::PBM_ASCII);
    argon::image_io::write("image_binary.pbm", pbm_image, argon::pnm_type::PBM_BINARY);

    argon::image<int> pgm_image(20, 20);
    argon::image<int> pgm_image_wide(20, 20);
    pgm_image( 9, 9) = 128;
    pgm_image(10,10) = 128;
    pgm_image( 9,10) = 128;
    pgm_image(10, 9) = 128;
    pgm_image_wide( 9, 9) = 32767;
    pgm_image_wide(10,10) = 32767;
    pgm_image_wide( 9,10) = 32767;
    pgm_image_wide(10, 9) = 32767;
    argon::image_io::write("image.pgm", pgm_image, argon::pnm_type::PGM_ASCII);
    argon::image_io::write("image_binary.pgm", pgm_image, argon::pnm_type::PGM_BINARY);
    argon::image_io::write("image_wide.pgm", pgm_image_wide, argon::pnm_type::PGM_ASCII);
    argon::image_io::write("image_wide_binary.pgm", pgm_image_wide, argon::pnm_type::PGM_BINARY);

    argon::image<int> ppm_image(20, 20, 3);
    ppm_image( 9, 9,0) = 128; ppm_image( 9, 9,1) = 64; ppm_image( 9, 9,2) = 255;
    ppm_image(10,10,0) = 128; ppm_image(10,10,1) = 64; ppm_image(10,10,2) = 255;
    ppm_image( 9,10,0) = 128; ppm_image( 9,10,1) = 64; ppm_image( 9,10,2) = 255;
    ppm_image(10, 9,0) = 128; ppm_image(10, 9,1) = 64; ppm_image(10, 9,2) = 255;
    argon::image_io::write("image.ppm", ppm_image, argon::pnm_type::PPM_ASCII);
}

#endif // AR_IMAGE_TEST_H_
