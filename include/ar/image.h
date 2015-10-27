#ifndef AR_IMAGE_H_
#define AR_IMAGE_H_

#include <vector>
#include <array>
#include <string>
#include <iostream>
#include <algorithm>
#include <stdexcept>
#include <cmath>

namespace argon
{
    template<typename T> class image
    {
        public:
            image( int width, int height, int channels = 1 )
                : _width{width}
                , _height{height}
                , _channels{channels}
                , _data(_width * _height * _channels, T(0))
            {}

            image( int width, int height, int channels, const std::vector<T> &data )
                : _width{width}
                , _height{height}
                , _channels{channels}
                , _data(data)
            {}

            image( int width, int height, int channels, std::vector<T> &&data )
                : _width{width}
                , _height{height}
                , _channels{channels}
                , _data(data)
            {}
       
            const T& operator()( int x, int y, int c = 0 ) const
            {
                return _data[c+_channels*(x+_width*y)];
            }

            T& operator()( int x, int y, int c = 0 )
            {
                return _data[c+_channels*(x+_width*y)];
            }

            const T& at( int x, int y, int c = 0 ) const
            {
                if (x < 0 || x >= _width || y < 0 || y >= _height || c < 0 || c >= _channels)
                    throw std::out_of_range(std::string("Attempted to access (" + std::to_string(x) + "," + std::to_string(y) + "," + std::to_string(c) + ")"));
                
                return _data[c+_channels*(x+_width*y)];
            }

            T& at( int x, int y, int c = 0)
            {
                if (x < 0 || x >= _width || y < 0 || y >= _height || c < 0 || c >= _channels)
                    throw std::out_of_range(std::string("Attempted to access (" + std::to_string(x) + "," + std::to_string(y) + "," + std::to_string(c) + ")"));

                return _data[c+_channels*(x+_width*y)];
            }

            T interpolate_linear(float x, float y, int c = 0)
            {
                int xf = std::floor( x );
                int xc = std::ceil( x );
                int yf = std::floor( y );
                int yc = std::ceil( y );

                x = x - xf; y = y - yf;

                float val = (*this)(xf,yf,c) * ( 1.f - x ) * ( 1.f - y );
                val  += (*this)(xc,yf,c) * x * ( 1.f - y );
                val  += (*this)(xf,yc,c) * ( 1.f - x ) * y;
                val  += (*this)(xc,yc,c) * x * y;

                return static_cast<T>(val);
            }

            int get_width() const { return _width; }
            int get_height() const { return _height; }
            int get_num_channels() const { return _channels; }

            T max_element() const
            {
                return *std::max_element(std::begin(_data), std::end(_data));
            }

            friend std::ostream& operator<<( std::ostream& out, const image<T> &img)
            {
                for (int y = 0; y < img.get_height(); ++y)
                {
                    for (int x = 0; x < img.get_width(); ++x)
                    {
                        out << '(';
                        bool first = true;
                        for (int c = 0; c < img.get_num_channels(); ++c)
                        {
                            if (!first)
                                out << ", ";
                            else
                                first = false;

                            out << img(x,y,c);
                        }
                        out << ") ";
                    }
                    out << "\n";
                }
                
                return out;
            }

            template <typename S>
            friend bool operator==( const image<S> &lhs, const image<S> &rhs );

            friend bool operator==( const image<float> &lhs, const image<float> &rhs );
            friend bool operator==( const image<double> &lhs, const image<double> &rhs );
            
            template <typename S>
            friend bool operator!=( const image<S> &lhs, const image<S> &rhs );

            friend bool operator!=( const image<float> &lhs, const image<float> &rhs );
            friend bool operator!=( const image<double> &lhs, const image<double> &rhs );
            const std::vector<T> & data() const
            {
                return _data;
            }

        protected:
            int _width;
            int _height;
            int _channels;
            std::vector<T> _data;
    };

    template <typename T> bool operator==( const image<T> &lhs, const image<T> &rhs )
    {
        if (lhs._width != rhs._width  ||
                lhs._height != rhs._height ||
                lhs._channels != lhs._channels)
            return false;

        for (auto p = 0u; p < lhs._data.size(); ++p)
        {
            if (lhs._data[p] != rhs._data[p])
                return false;
        }

        return true;
    }

    template <typename T> bool operator!=( const image<T> &lhs, const image<T> &rhs )
    {
        return !(lhs == rhs);
    }
}

#endif // AR_IMAGE_H_
