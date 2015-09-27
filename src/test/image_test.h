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
    argon::image<int> image(20, 20, 3);
    argon::image_io::write("image.ppm", image, argon::pnm_type::PPM_BINARY);
}

#endif // AR_IMAGE_TEST_H_
