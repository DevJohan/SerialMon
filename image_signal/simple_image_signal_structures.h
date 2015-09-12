/*
 * simple_image_signalStructures.h
 *
 *  Created on: 15 okt 2010
 *      Author: Johan
 */

#ifndef IMAGE_SIGNAL_STRUCTURES_H_
#define IMAGE_SIGNAL_STRUCTURES_H_

namespace _basic_image_signal {

template <typename T, bool is_const>
struct conditional_const{
	typedef T type;
};
template <typename T>
struct conditional_const<T,true>{
	typedef const T type;
};

template< typename T>
struct point2D{
	typedef T data_type;

	union{
		T coords[2];
		struct{
			T x,y;
		};
	};
	constexpr point2D( const data_type& _v ):x(_v),y(_v){}
	constexpr point2D( const data_type& _x, const data_type& _y ):x(_x),y(_y){}
};


template < typename Ta, typename Tb >
auto operator+( const point2D<Ta>&  p2d_a, const point2D<Tb>&  p2d_b) -> point2D<decltype(p2d_a.x+p2d_b.x)>{
	return { p2d_a.x + p2d_b.x, p2d_a.y + p2d_b.y };
}
template < typename Ta, typename Tb >
auto operator-( const point2D<Ta>&  p2d_a, const point2D<Tb>&  p2d_b) -> point2D<decltype(p2d_a.x-p2d_b.x)>{
	return { p2d_a.x - p2d_b.x, p2d_a.y - p2d_b.y };
}
template < typename Ta, typename Ts >
auto operator*( const point2D<Ta>&  p2d_a, const Ts&  s) -> point2D<decltype( p2d_a.x * s )>{
	return { p2d_a.x * s, p2d_a.y * s };
}
template < typename Ta, typename Ts >
auto operator*( const Ts&  s, const point2D<Ta>&  p2d_a) -> decltype( p2d_a * s ){
	return p2d_a * s;
}

template < typename Tr, typename Ta >
point2D<Tr> round( const point2D<Ta>&  p2d){
	return point2D<Tr>( round(p2d.x), round(p2d.y) );
}

template <typename T>
class range{
public:
	T min, max;
	range(const T& mi, const T& ma):min(mi),max(ma){}
	T lower_bound() const { return min; }
	T upper_bound() const { return max; }
};

template <typename T>
inline std::ostream& operator << (std::ostream& os, range<T> r ){
	return (os << "[" << r.min << "," << r.max << "]" );
}

struct image_area{
	typedef int size_type;
	typedef double position_type;
	typedef range<position_type> range_type;

	range_type range_x;
	range_type range_y;
};

inline std::ostream& operator << (std::ostream& os, image_area ia ){
	return (os << ia.range_x << ia.range_y );
}

}

#endif /* IMAGE_SIGNAL_STRUCTURES_H_ */
