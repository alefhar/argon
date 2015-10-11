#include "ar/pnm_types.h"
#include "ar/pnm_header_parser.h"

#include <stdexcept>

namespace argon
{
    void header_data::check()
    {
        if (magic != '1' && magic != '4')
            throw std::runtime_error("Unable to parse header, wrong magic number.");

        if (width <= 0)
            throw std::runtime_error("Unable to parse header, invalid width.");

        if (height <= 0)
            throw std::runtime_error("Unable to parse header, invalid height.");
    }

    void pnm_header::check()
    {
        header_data::check();

        if (max <= 0 || max >= 65535)
            throw std::runtime_error("Unable to parse header, invalid maximum.");
    }

    void pbm_header::check()
    {
        header_data::check();

        pnm_type type = static_cast<pnm_type>(magic);
        if (type != pnm_type::PBM_ASCII && type != pnm_type::PBM_BINARY)
            throw std::runtime_error("Unable to parse header, invalid type.");
    }

    void pgm_header::check()
    {
        pnm_header::check();

        pnm_type type = static_cast<pnm_type>(magic);
        if (type != pnm_type::PGM_ASCII && type != pnm_type::PGM_BINARY)
            throw std::runtime_error("Unable to parse header, invalid type.");
    }

    void ppm_header::check()
    {
        pnm_header::check();

        pnm_type type = static_cast<pnm_type>(magic);
        if (type != pnm_type::PPM_ASCII && type != pnm_type::PPM_BINARY)
            throw std::runtime_error("Unable to parse header, invalid type.");
    }

    void pfm_header::check()
    {
        header_data::check();
        
        pnm_type type = static_cast<pnm_type>(magic);
        if (type != pnm_type::PFM_SINGLE && type != pnm_type::PFM_TRIPLE)
            throw std::runtime_error("Unable to parse header, invalid type.");

        if (endianess != 1.f && endianess != -1.f)
            throw std::runtime_error("Unable to parse header, invalid endianess.");
    }

    std::ostream& operator<<( std::ostream &out, const pbm_header &header )
    {
        out << 'P' << header.magic << '\n';
        out << header.width << ' ' << header.height << '\n';

        return out;
    }

    std::istream& operator>>( std::istream &in, pbm_header &header)
    {
        pnm_header_parser parser;
        parser.parse_pbm_header(in, header);
        
        header.check();

        return in;
    }
        
    std::ostream& operator<<( std::ostream &out, const pgm_header &header )
    {
        out << 'P' << header.magic << '\n';
        out << header.width << ' ' << header.height << '\n';
        out << header.max << '\n';

        return out;
    }

    std::istream& operator>>( std::istream &in, pgm_header &header)
    {
        pnm_header_parser parser;
        parser.parse_pgm_header(in, header);

        header.check();

        header.bytes = header.max <= 255 ? 1 : 2;

        return in;
    }

    std::ostream& operator<<( std::ostream &out, const ppm_header &header )
    {
        out << 'P' << header.magic << '\n';
        out << header.width << ' ' << header.height << '\n';
        out << header.max << '\n';

        return out;
    }

    std::istream& operator>>( std::istream &in, ppm_header &header)
    {
        pnm_header_parser parser;
        parser.parse_ppm_header(in, header);

        header.check();

        header.bytes = header.max <= 255 ? 1 : 2;

        return in;
    }
            
    std::ostream& operator<<( std::ostream &out, const pfm_header &header )
    {

        out << 'P' << header.magic << '\n';
        out << header.width << ' ' << header.height << '\n';
        out << (header.endianess * header.scale) << '\n';

        return out;
    }
            
    std::istream& operator>>( std::istream &in, pfm_header &header )
    {
        pnm_header_parser parser;
        parser.parse_pfm_header(in, header);

        header.check();

        return in;
    }
}
