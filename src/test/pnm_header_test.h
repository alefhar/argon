#ifndef AR_PNM_HEADER_TEST_H
#define AR_PNM_HEADER_TEST_H

#include "ar/pnm_types.h"

#include <gtest/gtest.h>

#include <iostream>
#include <sstream>
#include <string>

TEST (pnm_header_test, pbm_test)
{
    std::string testee1 = "P1 5 7 ";
    std::string testee2 = "P4\n7\n9\n";
    std::string testee3 = "P1\n1 3 ";
    std::string testee4 = "#abc\nP1\n#def\n3\n#ghi\n2\n#jkl\n\n";
    std::string testee5 = "  #abc   \nP4\t  1\n   #def   #\n  3\n";
    std::string testee6 = "\rP1\r3\r5\r";
    std::string testee7 = "P1\n2 3\n1 0\n0 0\n1 1";

    std::stringstream sstream;
    argon::pbm_header header;

    sstream.str(testee1);
    sstream >> header;
    ASSERT_EQ('1', header.magic);
    ASSERT_EQ( 5 , header.width);
    ASSERT_EQ( 7 , header.height);

    sstream.str(testee2);
    sstream >> header;
    ASSERT_EQ('4', header.magic);
    ASSERT_EQ( 7 , header.width);
    ASSERT_EQ( 9 , header.height);

    sstream.str(testee3);
    sstream >> header;
    ASSERT_EQ('1', header.magic);
    ASSERT_EQ( 1 , header.width);
    ASSERT_EQ( 3 , header.height);

    sstream.str(testee4);
    sstream >> header;
    ASSERT_EQ('1', header.magic);
    ASSERT_EQ( 3 , header.width);
    ASSERT_EQ( 2 , header.height);

    sstream.str(testee5);
    sstream >> header;
    ASSERT_EQ('4', header.magic);
    ASSERT_EQ( 1 , header.width);
    ASSERT_EQ( 3 , header.height);

    sstream.str(testee6);
    sstream >> header;
    ASSERT_EQ('1', header.magic);
    ASSERT_EQ( 3 , header.width);
    ASSERT_EQ( 5 , header.height);

    sstream.str(testee7);
    sstream >> header;
    ASSERT_EQ('1', header.magic);
    ASSERT_EQ( 2 , header.width);
    ASSERT_EQ( 3 , header.height);
    ASSERT_EQ( 7 , sstream.tellg());
}

#endif // AR_PNM_HEADER_TEST_H
