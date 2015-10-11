#include "ar/pnm_types.h"
#include "ar/pnm_header_parser.h"

namespace argon
{
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

        return in;
    }
}
