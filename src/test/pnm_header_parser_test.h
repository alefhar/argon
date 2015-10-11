#ifndef AR_PNM_HEADER_PARSER_TEST_H
#define AR_PNM_HEADER_PARSER_TEST_H

#include "ar/pnm_types.h"
#include "ar/pnm_header_parser.h"

#include <gtest/gtest.h>

#include <sstream>

TEST (pnm_header_parser_test, parse_pbm_header)
{
    argon::pbm_header header;
    std::stringstream sstream;

    sstream.str("#feut\n P1 #euatro  \n#euanf\n \n\t\t\n\r\r    64 #aenutr \n   \t32\n");
    argon::pnm_header_parser parser;

    parser.parse_pbm_header(sstream, header);
    ASSERT_EQ('1', header.magic);
    ASSERT_EQ( 64, header.width);
    ASSERT_EQ( 32, header.height);
}

#endif /* end of include guard: AR_PNM_HEADER_PARSER_TEST_H */
