#ifndef AR_PNM_HEADER_PARSER_H
#define AR_PNM_HEADER_PARSER_H

#include "ar/pnm_types.h"

#include <iostream>

namespace argon
{
    class pnm_header_parser
    {
        private:
            enum class state { START, HAS_MAGIC, HAS_WIDTH, HAS_HEIGHT, HAS_MAX, HAS_ENDIANESS, COMPLETE, INVALID };
            
            state current_state;

        public:

            pnm_header_parser()
                : current_state(state::START)
            {}

            std::istream& parse_pbm_header( std::istream &in, pbm_header &header );

            std::istream& parse_pgm_header( std::istream &in, pgm_header &header );

            std::istream& parse_ppm_header( std::istream &in, ppm_header &header );

            std::istream& parse_pfm_header( std::istream &in, pfm_header &header );

        private:

            bool consume_whitespace( std::istream &in );

            bool consume_comment( std::istream &in );

            void filter( std::istream &in);

            void consume_magic( std::istream &in, pnm_header &header );

            void consume_width( std::istream &in, pnm_header &header );

            void consume_height( std::istream &in, pnm_header &header );

            void consume_max( std::istream &in, pnm_header &header );

            void consume_endianess( std::istream &in, pfm_header &header );
    };
}

#endif /* end of include guard: AR_PNM_HEADER_PARSER_H */
