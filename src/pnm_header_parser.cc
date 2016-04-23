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

#include "ar/pnm_header_parser.h"

#include <string>
#include <sstream>
#include <limits>
#include <stdexcept>

namespace argon
{
    
    std::istream& pnm_header_parser::parse_pbm_header( std::istream &in, pbm_header &header )
	{
        while (current_state != state::COMPLETE && current_state != state::INVALID)
        {
            switch (current_state)
            {
                case state::START:
                    filter(in);
                    consume_magic(in, header);
                    break;
                
                case state::HAS_MAGIC:
                    filter(in);
                    consume_width(in, header);
                    break;

                case state::HAS_WIDTH:
                    filter(in);
                    consume_height(in, header);
                    break;

                case state::HAS_HEIGHT:
                    {
                        char delim = in.get();
                        if (delim == '\n' || delim == ' ' || delim == '\t' || delim == '\r')
                            current_state = state::COMPLETE;
                        else
                            current_state = state::INVALID;
                    }
                    break;

                default:
                    current_state = state::INVALID;
            }
        }

        if (current_state == state::INVALID)
            throw std::runtime_error("Could not parse header");

        return in;
	}

    std::istream& pnm_header_parser::parse_pgm_header( std::istream &in, pgm_header &header )
	{
        return parse_pgm_ppm_header(in, header);
	}

    std::istream& pnm_header_parser::parse_ppm_header( std::istream &in, ppm_header &header )
	{
        return parse_pgm_ppm_header(in, header);
	}

    std::istream& pnm_header_parser::parse_pgm_ppm_header( std::istream &in, pnm_header &header )
    {
        while (current_state != state::COMPLETE && current_state != state::INVALID)
        {
            switch (current_state)
            {
                case state::START:
                    filter(in);
                    consume_magic(in, header);
                    break;
                
                case state::HAS_MAGIC:
                    filter(in);
                    consume_width(in, header);
                    break;

                case state::HAS_WIDTH:
                    filter(in);
                    consume_height(in, header);
                    break;

                case state::HAS_HEIGHT:
                    filter(in);
                    consume_max(in, header);
                    break;

                case state::HAS_MAX:
                    {
                        char delim = in.get();
                        if (delim == '\n' || delim == ' ' || delim == '\t' || delim == '\r')
                            current_state = state::COMPLETE;
                        else
                            current_state = state::INVALID;
                    }
                    break;

                default:
                    current_state = state::INVALID;
            }
        }

        if (current_state == state::INVALID)
            throw std::runtime_error("Could not parse header");

        return in;
    }

    std::istream& pnm_header_parser::parse_pfm_header( std::istream &in, pfm_header &header )
	{
        while (current_state != state::COMPLETE && current_state != state::INVALID)
        {
            switch (current_state)
            {
                case state::START:
                    filter(in);
                    consume_magic(in, header);
                    break;
                
                case state::HAS_MAGIC:
                    filter(in);
                    consume_width(in, header);
                    break;

                case state::HAS_WIDTH:
                    filter(in);
                    consume_height(in, header);
                    break;

                case state::HAS_HEIGHT:
                    filter(in);
                    consume_endianess(in, header);
                    break;

                case state::HAS_ENDIANESS:
                    {
                        char delim = in.get();
                        if (delim == '\n' || delim == ' ' || delim == '\t' || delim == '\r')
                            current_state = state::COMPLETE;
                        else
                            current_state = state::INVALID;
                    }
                    break;

                default:
                    current_state = state::INVALID;
            }
        }

        if (current_state == state::INVALID)
            throw std::runtime_error("Could not parse header");

        return in;
	}

    bool pnm_header_parser::consume_whitespace( std::istream &in )
	{
        char front = in.get();
        bool consumed = false;
        while (front == '\n' || front == ' ' || front == '\r' || front == '\t')
        {
            front = in.get();
            consumed = true;
        }

        in.unget();

        return consumed;
	}

    bool pnm_header_parser::consume_comment( std::istream &in )
	{
        char front = in.get();
        bool consumed = false;
        if (front == '#')
        {
            in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            consumed = true;
        }
        else
        {
            in.unget();
        }

        return consumed;
	}

    void pnm_header_parser::filter( std::istream &in )
    {
        bool whitespace_consumed = true, comment_consumed = true;
        while (whitespace_consumed || comment_consumed)
        {
            whitespace_consumed = consume_whitespace(in);
            comment_consumed = consume_comment(in);
        }
    }

    void pnm_header_parser::consume_magic( std::istream &in, header_data &header )
	{
        char front = in.get();
        if (front == 'P') {
            header.magic = in.get();
            current_state = state::HAS_MAGIC;
        }
        else
        {
            current_state = state::INVALID;
        }
	}

    void pnm_header_parser::consume_width( std::istream &in, header_data &header )
	{
        in >> header.width;
        current_state = state::HAS_WIDTH;
	}

    void pnm_header_parser::consume_height( std::istream &in, header_data &header )
	{
        in >> header.height;
        current_state = state::HAS_HEIGHT;
	}

    void pnm_header_parser::consume_max( std::istream &in, pnm_header &header )
    {
        in >> header.max;
        current_state = state::HAS_MAX;
    }

    void pnm_header_parser::consume_endianess( std::istream &in, pfm_header &header )
	{
        float val;
        in >> val;
        header.endianess = val < 0.f ? -1.f : 1.f;
        header.scale = std::abs(val);

        current_state = state::HAS_ENDIANESS;
	}
}
