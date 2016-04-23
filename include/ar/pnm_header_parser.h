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

            void reset()
            {
                current_state = state::START;
            }

            std::istream& parse_pbm_header( std::istream &in, pbm_header &header );

            std::istream& parse_pgm_header( std::istream &in, pgm_header &header );

            std::istream& parse_ppm_header( std::istream &in, ppm_header &header );

            std::istream& parse_pfm_header( std::istream &in, pfm_header &header );

        private:

            std::istream& parse_pgm_ppm_header( std::istream &in, pnm_header &header );

            bool consume_whitespace( std::istream &in );

            bool consume_comment( std::istream &in );

            void filter( std::istream &in);

            void consume_magic( std::istream &in, header_data &header );

            void consume_width( std::istream &in, header_data &header );

            void consume_height( std::istream &in, header_data &header );

            void consume_max( std::istream &in, pnm_header &header );

            void consume_endianess( std::istream &in, pfm_header &header );
    };
}

#endif /* end of include guard: AR_PNM_HEADER_PARSER_H */
