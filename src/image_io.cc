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

#include "ar/image_io.h"
#include "ar/image.h"

#include <fstream>
#include <iostream>

namespace argon
{
    pnm_type image_io::peek_header( const std::string &filename )
    {
        std::ifstream in(filename, std::ios::in);
        if (!in.is_open())
           throw std::runtime_error("Could not open " + filename);

        char magic[2];
        in.read(&magic[0], 2);

        return static_cast<pnm_type>(magic[1]);
    }

    pbm_header image_io::read_pbm_header( std::ifstream &in )
    {
        pbm_header header;
        in >> header;
        return header;
    }

    pgm_header image_io::read_pgm_header( std::ifstream &in )
    {
        pgm_header header;
        in >> header;
        return header;
    }

    ppm_header image_io::read_ppm_header( std::ifstream &in )
    {
        ppm_header header;
        in >> header;
        return header;
    }

    pfm_header image_io::read_pfm_header( std::ifstream &in )
    {
        pfm_header header;
        in >> header;
        return header;
    }
}
