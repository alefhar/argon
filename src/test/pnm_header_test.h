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

#ifndef AR_PNM_HEADER_TEST_H
#define AR_PNM_HEADER_TEST_H

#include "ar/pnm_types.h"

#include <gtest/gtest.h>

#include <iostream>
#include <sstream>
#include <string>
#include <stdexcept>

TEST (pnm_header_test, pbm_test)
{
    std::string testee1  = "P1 5 7 ";
    std::string testee2  = "P4\n7\n9\n";
    std::string testee3  = "P1\n1 3 ";
    std::string testee4  = "#abc\nP1\n#def\n3\n#ghi\n2\n#jkl\n\n";
    std::string testee5  = "  #abc   \nP4\t  1\n   #def   #\n  3\n";
    std::string testee6  = "\rP1\r3\r5\r";
    std::string testee7  = "P1\n2 3\n1 0\n0 0\n1 1";
    std::string testee8  = "P2 -1 0\n";
    std::string testee9  = "P1 -1 0\n";
    std::string testee10 = "P1 5 0\n";

    std::stringstream sstream;
    argon::pbm_header header;

    sstream.str(testee1);
    sstream >> header;
    EXPECT_EQ('1', header.magic);
    EXPECT_EQ( 5 , header.width);
    EXPECT_EQ( 7 , header.height);

    sstream.str(testee2);
    sstream >> header;
    EXPECT_EQ('4', header.magic);
    EXPECT_EQ( 7 , header.width);
    EXPECT_EQ( 9 , header.height);

    sstream.str(testee3);
    sstream >> header;
    EXPECT_EQ('1', header.magic);
    EXPECT_EQ( 1 , header.width);
    EXPECT_EQ( 3 , header.height);

    sstream.str(testee4);
    sstream >> header;
    EXPECT_EQ('1', header.magic);
    EXPECT_EQ( 3 , header.width);
    EXPECT_EQ( 2 , header.height);

    sstream.str(testee5);
    sstream >> header;
    EXPECT_EQ('4', header.magic);
    EXPECT_EQ( 1 , header.width);
    EXPECT_EQ( 3 , header.height);

    sstream.str(testee6);
    sstream >> header;
    EXPECT_EQ('1', header.magic);
    EXPECT_EQ( 3 , header.width);
    EXPECT_EQ( 5 , header.height);

    sstream.str(testee7);
    sstream >> header;
    EXPECT_EQ('1', header.magic);
    EXPECT_EQ( 2 , header.width);
    EXPECT_EQ( 3 , header.height);
    EXPECT_EQ( 7 , sstream.tellg());

    sstream.str(testee8);
    EXPECT_THROW(sstream >> header, std::runtime_error);

    sstream.str(testee9);
    EXPECT_THROW(sstream >> header, std::runtime_error);

    sstream.str(testee10);
    EXPECT_THROW(sstream >> header, std::runtime_error);
}

TEST (pnm_header_test, pgm_test)
{
    std::string testee1 = "P2 16 32 255\n";
    std::string testee2 = "P5 32 16 65535\n";
    std::string testee3 = "P1 -1 0 128000\n";
    std::string testee4 = "P2 -1 0 128000\n";
    std::string testee5 = "P2 16 0 128000\n";
    std::string testee6 = "P2 16 32 128000\n";

    std::stringstream sstream;
    argon::pgm_header header;

    sstream.str(testee1);
    sstream >> header;
    EXPECT_EQ('2', header.magic);
    EXPECT_EQ( 16, header.width);
    EXPECT_EQ( 32, header.height);
    EXPECT_EQ(255, header.max);

    sstream.str(testee2);
    sstream >> header;
    EXPECT_EQ('5', header.magic);
    EXPECT_EQ( 32, header.width);
    EXPECT_EQ( 16, header.height);
    EXPECT_EQ(65535, header.max);

    sstream.str(testee3);
    EXPECT_THROW(sstream >> header, std::runtime_error);
    
    sstream.str(testee4);
    EXPECT_THROW(sstream >> header, std::runtime_error);
    
    sstream.str(testee5);
    EXPECT_THROW(sstream >> header, std::runtime_error);
    
    sstream.str(testee6);
    EXPECT_THROW(sstream >> header, std::runtime_error);
}

TEST (pnm_header_test, ppm_test)
{
    std::string testee1 = "P3 16 32 255\n";
    std::string testee2 = "P6 32 16 65535\n";
    std::string testee3 = "P1 -1 0 128000\n";
    std::string testee4 = "P3 -1 0 128000\n";
    std::string testee5 = "P3 16 0 128000\n";
    std::string testee6 = "P3 16 32 128000\n";

    std::stringstream sstream;
    argon::ppm_header header;

    sstream.str(testee1);
    sstream >> header;
    EXPECT_EQ('3', header.magic);
    EXPECT_EQ( 16, header.width);
    EXPECT_EQ( 32, header.height);
    EXPECT_EQ(255, header.max);

    sstream.str(testee2);
    sstream >> header;
    EXPECT_EQ('6', header.magic);
    EXPECT_EQ( 32, header.width);
    EXPECT_EQ( 16, header.height);
    EXPECT_EQ(65535, header.max);

    sstream.str(testee3);
    EXPECT_THROW(sstream >> header, std::runtime_error);
    
    sstream.str(testee4);
    EXPECT_THROW(sstream >> header, std::runtime_error);
    
    sstream.str(testee5);
    EXPECT_THROW(sstream >> header, std::runtime_error);
    
    sstream.str(testee6);
    EXPECT_THROW(sstream >> header, std::runtime_error);
}

TEST (pnm_header_test, pfm_test)
{
    std::string testee1 = "Pf 16 32 -1.0\n";
    std::string testee2 = "PF 32 16 1.\n";
    std::string testee3 = "P1 -1 0 2.\n";
    std::string testee4 = "PF -1 0 2.\n";
    std::string testee5 = "PF 16 0 2.\n";

    std::stringstream sstream;
    argon::pfm_header header;

    sstream.str(testee1);
    sstream >> header;
    EXPECT_EQ( 'f', header.magic);
    EXPECT_EQ(  16, header.width);
    EXPECT_EQ(  32, header.height);
    EXPECT_EQ(-1.f, header.endianess);

    sstream.str(testee2);
    sstream >> header;
    EXPECT_EQ('F', header.magic);
    EXPECT_EQ( 32, header.width);
    EXPECT_EQ( 16, header.height);
    EXPECT_EQ(1.f, header.endianess);

    sstream.str(testee3);
    EXPECT_THROW(sstream >> header, std::runtime_error);
    
    sstream.str(testee4);
    EXPECT_THROW(sstream >> header, std::runtime_error);
    
    sstream.str(testee5);
    EXPECT_THROW(sstream >> header, std::runtime_error);
}

#endif // AR_PNM_HEADER_TEST_H
