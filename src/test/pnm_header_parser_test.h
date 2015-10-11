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
    std::string testee3 = "#feut\n P1 #euatro  \n#euanf\n \n\t\t\n\r\r    64 #aenutr \n   \t32\n";
    std::string testee4 = "P1 16.5 24.8\n";
    
    std::stringstream sstream;
    
    sstream.str(testee1);
    parser.reset();
    parser.parse_pbm_header(sstream, header);
    EXPECT_EQ('1', header.magic);
    EXPECT_EQ( 32, header.width);
    EXPECT_EQ( 64, header.height);
    
    sstream.str(testee3);
    parser.reset();
    parser.parse_pbm_header(sstream, header);
    EXPECT_EQ('1', header.magic);
    EXPECT_EQ( 64, header.width);
    EXPECT_EQ( 32, header.height);

    sstream.str(testee4);
    parser.reset();
    EXPECT_THROW(parser.parse_pbm_header(sstream, header), std::runtime_error);
}

#endif /* end of include guard: AR_PNM_HEADER_PARSER_TEST_H */
