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
                    //std::cout << "start" << std::endl;
                    filter(in);
                    consume_magic(in, header);
                    break;
                
                case state::HAS_MAGIC:
                    //std::cout << "has_magic" << std::endl;
                    filter(in);
                    consume_width(in, header);
                    break;

                case state::HAS_WIDTH:
                    //std::cout << "has_width" << std::endl;
                    filter(in);
                    consume_height(in, header);
                    break;

                case state::HAS_HEIGHT:
                    //std::cout << "has_height" << std::endl;
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
        return in;
	}

    std::istream& pnm_header_parser::parse_ppm_header( std::istream &in, ppm_header &header )
	{
        return in;
	}

    std::istream& pnm_header_parser::parse_pfm_header( std::istream &in, pfm_header &header )
	{
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
            //std::cout << static_cast<char>(in.peek()) << " " << in.peek() << std::endl;
            whitespace_consumed = consume_whitespace(in);
            comment_consumed = consume_comment(in);
        }
    }

    void pnm_header_parser::consume_magic( std::istream &in, pnm_header &header )
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

    void pnm_header_parser::consume_width( std::istream &in, pnm_header &header )
	{
        std::stringstream sstream;
        char front = in.get();
        while (front >= '0' && front <= '9')
        {
            //std::cout << front << std::endl;
            sstream << front;
            front = in.get();
        }

        auto width = sstream.str();
        if (!width.empty())
        {
            header.width = std::stoi(sstream.str());
            current_state = state::HAS_WIDTH;
            in.unget();
        }
        else
        {
            current_state = state::INVALID;
        }
	}

    void pnm_header_parser::consume_height( std::istream &in, pnm_header &header )
	{
        std::stringstream sstream;
        char front = in.get();
        while (front >= '0' && front <= '9')
        {
            sstream << front;
            front = in.get();
        }

        auto height = sstream.str();
        if (!height.empty())
        {
            header.height = std::stoi(sstream.str());
            current_state = state::HAS_HEIGHT;
            in.unget();
        }
        else
        {
            current_state = state::INVALID;
        }
	}

    void pnm_header_parser::consume_max( std::istream &in, pnm_header &header )
    {
    }

    void pnm_header_parser::consume_endianess( std::istream &in, pfm_header &header )
	{
	}
}
