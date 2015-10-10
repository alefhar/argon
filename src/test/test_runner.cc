#include "gtest/gtest.h"

#include "image_test.h"
#include "pnm_header_test.h"
#include "endian_test.h"

int main(int argc, char **argv)
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();  
}
