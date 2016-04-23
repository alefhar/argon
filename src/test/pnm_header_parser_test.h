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

#ifndef AR_PNM_HEADER_PARSER_TEST_H
#define AR_PNM_HEADER_PARSER_TEST_H

#include "ar/pnm_types.h"
#include "ar/pnm_header_parser.h"

#include <gtest/gtest.h>

#include <string>
#include <sstream>
#include <stdexcept>

TEST (pnm_header_parser_test, parse_pbm_header)
{
    argon::pbm_header header;
    argon::pnm_header_parser parser;
    
    std::string testee1 = "P1 32 64\n";
    std::string testee2 = "\rP4\t128\n256 "; 
    std::string testee3 = "#feut\n P1 #euatro  \n#euanf\n \n\t\t\n\r\r    64 #aenutr \n   \t32\n";
    std::string testee4 = "P1 16.5 24.8\n";
    std::string testee5 = "P235 16 32\n";
    
    std::stringstream sstream;
    
    sstream.str(testee1);
    parser.reset();
    parser.parse_pbm_header(sstream, header);
    EXPECT_EQ('1', header.magic);
    EXPECT_EQ( 32, header.width);
    EXPECT_EQ( 64, header.height);

    sstream.str(testee2);
    parser.reset();
    parser.parse_pbm_header(sstream, header);
    EXPECT_EQ('4', header.magic);
    EXPECT_EQ(128, header.width);
    EXPECT_EQ(256, header.height);
    
    sstream.str(testee3);
    parser.reset();
    parser.parse_pbm_header(sstream, header);
    EXPECT_EQ('1', header.magic);
    EXPECT_EQ( 64, header.width);
    EXPECT_EQ( 32, header.height);

    sstream.str(testee4);
    parser.reset();
    EXPECT_THROW(parser.parse_pbm_header(sstream, header), std::runtime_error);

    sstream.str(testee5);
    parser.reset();
    EXPECT_THROW(parser.parse_pbm_header(sstream, header), std::runtime_error);
}

TEST (pnm_header_parser_test, parse_pgm_header)
{
    argon::pgm_header header;
    argon::pnm_header_parser parser;
    
    std::string testee1 = "P2 32 64 255\n";
    
    std::stringstream sstream;
    
    sstream.str(testee1);
    parser.reset();
    parser.parse_pgm_header(sstream, header);
    EXPECT_EQ('2', header.magic);
    EXPECT_EQ( 32, header.width);
    EXPECT_EQ( 64, header.height);
    EXPECT_EQ(255, header.max);
}

TEST (pnm_header_parser_test, parse_ppm_header)
{
    argon::ppm_header header;
    argon::pnm_header_parser parser;
    
    std::string testee1 = "P3 32 64 255\n";
    
    std::stringstream sstream;
    
    sstream.str(testee1);
    parser.reset();
    parser.parse_ppm_header(sstream, header);
    EXPECT_EQ('3', header.magic);
    EXPECT_EQ( 32, header.width);
    EXPECT_EQ( 64, header.height);
    EXPECT_EQ(255, header.max);
}

TEST (pnm_header_parser_test, parse_pfm_header)
{
    argon::pfm_header header;
    argon::pnm_header_parser parser;
    
    std::string testee1 = "Pf 32 64 -1.0\n";
    std::string testee2 = "PF 64 32 1.0\n";
    
    std::stringstream sstream;
    
    sstream.str(testee1);
    parser.reset();
    parser.parse_pfm_header(sstream, header);
    EXPECT_EQ('f' , header.magic);
    EXPECT_EQ( 32 , header.width);
    EXPECT_EQ( 64 , header.height);
    EXPECT_EQ(-1.f, header.endianess);
    
    sstream.str(testee2);
    parser.reset();
    parser.parse_pfm_header(sstream, header);
    EXPECT_EQ('F', header.magic);
    EXPECT_EQ( 64, header.width);
    EXPECT_EQ( 32, header.height);
    EXPECT_EQ(1.f, header.endianess);
}

#endif /* end of include guard: AR_PNM_HEADER_PARSER_TEST_H */
