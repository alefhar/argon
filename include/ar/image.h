#ifndef AR_IMAGE_H_
#define AR_IMAGE_H_

#include <vector>
#include <array>
#include <string>
#include <algorithm>
#include <stdexcept>
#include <cmath>

namespace argon
{
    template<typename T> class image
    {
        public:
            image( int width, int height, int channels = 1 )
                : _width{width}, _height{height}, _channels{channels},
                  _data(_width * _height * _channels, T(0))
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

        public:
            
        protected:
            int _width;
            int _height;
            int _channels;
            std::vector<T> _data;
    };
}

#endif // AR_IMAGE_H_
