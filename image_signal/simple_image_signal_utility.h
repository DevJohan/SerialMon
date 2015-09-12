/*
 * UtilityFunctions.h
 *
 *  Created on: 1 sep 2010
 *      Author: Johan
 */

#ifndef IMAGE_SIGNAL_UTILITY_FUNCTIONS_H_
#define IMAGE_SIGNAL_UTILITY_FUNCTIONS_H_

#include <cmath>
#include <type_traits>

#include "color_types.h"
#include "simple_image_signal.h"
#include "simple_image_signal_draw.h"
#include "simple_image_signal_structures.h"
#include "simple_image_signal_function.h"
#include "utility.h"


namespace _basic_image_signal {

template<class T, class CT>
void drawPlot(
		const T* const src,
		const int size,
		basic_image_signal<CT>& dst,
		CT const color
){
	const int width = dst.raw_width();
	const int height = dst.raw_height();

	T total_max = 0;
	T total_min = 0;
	array_max_min(&total_max, &total_min, src, size);

	if(total_max-total_min == 0)return;

	const double scale = (height-1) / (total_max - total_min);
	const double offset = (height-1) + scale*total_min;
	/***
	 * i: index of image, dst
	 * source_index_last: index of array, src
	 *
	 *	number of steps: width + 1
	 *	width pixels and one extra when counting pixel sides
	 *
	 */

	double step_size = static_cast<double>(size) / (width + 1);
	double current_source_index = step_size;

	int source_index_last = 0;
	T interpolated_value_last = src[0];

	for(int i = 0; i<width;i++){
		const int source_index = static_cast<int>(current_source_index);
		const double remainder = _basic_image_signal::euclidean_remainder(current_source_index,1.0);

		const int next_source_index = source_index + 1 < size ? source_index + 1 : source_index;
		const T interpolated_value = src[source_index]*(1 - remainder) + src[next_source_index]*remainder;
		const bool larger_than_last = interpolated_value > interpolated_value_last;
		T region_max = larger_than_last ? interpolated_value : interpolated_value_last;
		T region_min = larger_than_last ? interpolated_value_last : interpolated_value;

		const int source_span = source_index - source_index_last;
		if(source_span > 0){
			T partial_max;
			T partial_min;
			array_max_min(&partial_max, &partial_min, src + source_index, source_span);
			region_max = std::max(region_max, partial_max);
			region_min = std::min(region_min, partial_min);
		}


		drawVerticalLine(dst,i,offset -scale*region_max, offset - scale*region_min, color);

		current_source_index += step_size;
		source_index_last = source_index;
		interpolated_value_last = interpolated_value;
	}
}

template<class T, class CT>
void drawPlot(
		const T* const index,
		const T* const src,
		const int size,
		basic_image_signal<CT>& dst,
		CT const color
){
	const int width = dst.raw_width();
	const int height = dst.raw_height();

	T total_max_y = 0;
	T total_min_y = 0;
	array_max_min(&total_max_y, &total_min_y, src, size);

	const double scale_y = -(height-1) / (total_max_y - total_min_y);
	const double offset_y = (height-1) - scale_y*total_min_y;

	T total_max_x = 0;
	T total_min_x = 0;
	array_max_min(&total_max_x, &total_min_x, index, size);

	const double scale_x = (width -1) / (total_max_x - total_min_x);
	const double offset_x = -scale_x*total_min_x;

	double last_x = offset_x + scale_x*index[0];
	double last_y = offset_y + scale_y*src[0];

	for(int i = 1; i<size;i++){
		const double x = offset_x + scale_x*index[i];
		const double y = offset_y + scale_y*src[i];
		drawLine(dst, last_x, last_y, x, y, color);
		last_x = x;
		last_y = y;
	}
}

template<class T, class CT>
void drawPlotAxisEqual(
		const T* const index,
		const T* const src,
		const int size,
		basic_image_signal<CT>& dst,
		CT const color
){
    const int width = dst.raw_width();
    const int height = dst.raw_height();

    T total_max_y = 0;
    T total_min_y = 0;
    array_max_min(&total_max_y, &total_min_y, src, size);

    double scale_y = -(height-1) / (total_max_y - total_min_y);

    T total_max_x = 0;
    T total_min_x = 0;
    array_max_min(&total_max_x, &total_min_x, index, size);

    double scale_x = (width -1) / (total_max_x - total_min_x);

//    cout << "scale_x = " << scale_x << endl;
//    cout << "scale_y = " << scale_y << endl;

    double offset_y=0;
    double offset_x=0;
    if(scale_x>-scale_y){
        scale_x=-scale_y;
        offset_y = (height-1) - scale_y*total_min_y;
        offset_x = width*0.5 -scale_x*((total_max_x-total_min_x)*0.5+total_min_x);}
    else{
        scale_y=-scale_x;
        offset_y = height*0.5 - scale_y*((total_max_y-total_min_y)*0.5+total_min_y);
        offset_x = -scale_x*total_min_x;
    }

    double last_x = offset_x + scale_x*index[0];
    double last_y = offset_y + scale_y*src[0];

//    cout << "scale_x = " << scale_x << endl;
//    cout << "scale_y = " << scale_y << endl;
//    cout << "offset_x = " << offset_x << endl;
//    cout << "offset_y = " << offset_y << endl;

    for( int i = 1; i < size; i++ ){
        const double x = offset_x + scale_x*index[i];
        const double y = offset_y + scale_y*src[i];
        drawLine(dst, last_x, last_y, x, y, color);
        last_x = x;
        last_y = y;
    }
}






template< typename D >
void clear_image(
		basic_image_signal<D>& im
){
	typedef typename basic_image_signal<D>::size_type size_type;
	D* data = im.row_data(0);
	const int step = im.width_step() ;

	for( size_type n = 0; n < im.raw_height(); n++){
		D* d = data;
		for( size_type m = 0; m < im.raw_width(); m++){
			*d++ = 0;
		}
		data += step;
	}
}


template<class D>
void clear_to(
		basic_image_signal<D>& im,
		const D& c
){

	for(int n=0; n < im.raw_height(); n++){
		D* d = im.row_data(n);
		for(int m=0; m < im.raw_width(); m++){
			*d++ = c;
		}
	}
}

template<class D>
typename std::enable_if<std::is_integral<D>::value,void>::type
downshift_image(
		basic_image_signal<D>& im,
		const size_t shift
){
	for(int n=0; n < im.raw_height(); n++){
		D* d = im.row_data(n);
		for(int m=0; m < im.raw_width(); m++){
			*d++ >>= shift;
		}
	}
}

template<class S, class D>
void subsample_image(
		const basic_image_signal<S>& src,
		basic_image_signal<D>& dst
){
	const int dst_width = src.raw_width()/2;
	const int dst_height = src.raw_height()/2;
	dst.set_raw_size( dst_width, dst_height );

	for(int n=0; n < dst_height; n++){
		const S* s = src.row_data(2*n);
		D* d = dst.row_data(n);
		for(int m=0; m < dst_width; m++){
			*d = *s;
			d++;
			s+=2;
		}
	}
	dst.offset() = src.offset();
	dst.sample_factor( 2 * src.sample_factor() );
}

template< class D >
void subsample_image(
		basic_image_signal<D>& dst
){
	const int dst_width = ( dst.raw_width() + 1 ) >> 1;
	const int dst_height = ( dst.raw_height() + 1 ) >> 1;

	for( int n=0; n < dst_height; n++){
		const D* src = dst[2*n];
		D* d = dst[n];
		for( int m=0; m < dst_width; m++){
			*d = *src;
			d++;
			src+=2;
		}
	}
	dst.raw_width( dst_width );
	dst.raw_height( dst_height );
	dst.sample_factor( 2 * dst.sample_factor() );
}



template<class D>
void add_image(
		basic_image_signal<D>& dstIm,
		const D value
){

	for(int n=0; n < dstIm->raw_height(); n++){
		D* d = dstIm.row_data(n);
		for(int m=0; m < dstIm->raw_width(); m++){
			*d++ += value;
		}
	}
}

template<class D,class S>
void add_image(
		basic_image_signal<D>& dstIm,
		const basic_image_signal<S>& srcIm
){
	for(int n=0; n < dstIm->raw_height(); n++){
		D* d = dstIm.row_data(n);
		const S* s = srcIm.row_data(n);
		for(int m=0; m < dstIm->raw_width(); m++){
			*d++ += *s++;
		}
	}
}

template<class D,class S>
void sub_image(
		basic_image_signal<D>& dst_im,
		const basic_image_signal<S>& subIm,
		basic_image_signal<D>& dst_im2
){
	for(int n=0; n < dst_im.raw_height(); n++){
		D* d = dst_im.row_data(n);
		const S* s = subIm.row_data(n);
		for(int m=0; m < dst_im.raw_width(); m++){
			*d++ -= *s++;
		}
	}
}

template<class D>
void scale_image(
		basic_image_signal<D>& im,
		double scale
){
	for(int n=0; n < im.raw_height(); n++){
		D* d = im.row_data(n);
		for(int m=0; m < im.raw_width(); m++){
			*d++ *= scale;
		}
	}
}

template<class S, class D>
inline void copy_image_meta_data(
		const basic_image_signal<S>& src,
		basic_image_signal<D>& dst
){
	dst.set_raw_size( src.raw_width(), src.raw_height() );
	dst.offset( src.offset() );
	dst.sample_factor( src.sample_factor());
}

template<class S, class D>
void copy_scale_image(
		const basic_image_signal<S>& src,
		basic_image_signal<D>& dst,
		double scale
){
	typedef typename basic_image_signal<S>::size_type size_type;
	copy_image_meta_data( src, dst );

	for(size_type n=0; n < src.raw_height(); n++){
		const S* s = src.row_data(n);
		D* d = dst.row_data(n);
		for(size_type m=0; m < src.raw_width(); m++){
			*d++ = scale*(*s++);
		}
	}
}

template<class S,class D>
void copy_image(
		const basic_image_signal<S>& src,
		basic_image_signal<D>& dst
){
	typedef typename basic_image_signal<S>::size_type size_type;
	copy_image_meta_data( src, dst );

	for(size_type i = 0; i<src.raw_height(); i++){
		const S* s = src.row_data(i);
		D* d = dst.row_data(i);

		for(size_type j = 0; j<src.raw_width(); j++){
			*d++ = *s++;
		}
	}
}

template<class S>
inline S bilinear_interpolation(
		const basic_image_signal<S>& s_im,
		const image_area::position_type& px,
		const image_area::position_type& py
){
	const int x = px;
	const int y = py;

	const float dx = px-x;
	const float dy = py-y;

	const S p1 = s_im[y][x]*(1.f-dx) + s_im[y][x+1]*dx;
	const S p2 = s_im[y+1][x]*(1.f-dx) + s_im[y+1][x+1]*dx;
	return p1*(1.f-dy) + p2*dy;
}

typedef double image_length;

template<class S,class D>
void copy_into_image(
		const basic_image_signal<S>& src,
		basic_image_signal<D>& dst,
		const image_area& dstArea
){
	const int start_y = dstArea.range_y.lower_bound();
	const int stop_y = dstArea.range_y.upper_bound();
	const int start_x = dstArea.range_x.lower_bound();
	const int stop_x = dstArea.range_y.upper_bound();

	image_area::position_type pos_x = 0, pos_y = 0;
	const image_length step_x = (src.raw_width() - 1) / (image_length)(stop_x - start_x);
	const image_length step_y = (src.raw_height() - 1)/ (image_length)(stop_y - start_y);

	for(int i = start_y; i<stop_y; i++){
		D* d = dst[ i ];
		pos_x = 0;

		for(int j = start_x; j<stop_x; j++){
			d[j] = bilinear_interpolation(src, pos_x, pos_y);
			pos_x += step_x;
		}
		pos_y += step_y;
	}
}

template<class S,class D>
void copy_into_image(
		const basic_image_signal<S>& src,
		basic_image_signal<D>& dst,
		const image_area& dstArea,
		const image_area& srcArea
){
	const int start_y = dstArea.range_y.lower_bound();
	const int stop_y = dstArea.range_y.upper_bound();
	const int start_x = dstArea.range_x.lower_bound();
	const int stop_x = dstArea.range_y.upper_bound();

	const int src_start_y = srcArea.range_y.lower_bound() > 0 ? srcArea.range_y.lower_bound() : 0;
	const int src_stop_y = srcArea.range_y.upper_bound() < src.raw_height() - 1 ? srcArea.range_y.upper_bound() : src.raw_height() - 1;
	const int src_start_x = srcArea.range_x.lower_bound() > 0 ? srcArea.range_x.lower_bound() : 0;
	const int src_stop_x = srcArea.range_x.upper_bound() < src.raw_width() - 1 ? srcArea.range_x.upper_bound() : src.raw_width() - 1;

	image_area::position_type pos_x = src_start_x, pos_y = src_start_y;
	const image_length step_x = (src_stop_x - src_start_x) / (image_length)(stop_x - start_x);
	const image_length step_y = (src_stop_y - src_start_y) / (image_length)(stop_y - start_y);

	for(int i = start_y; i<stop_y; i++){
		D* d = dst[i];
		pos_x = 0;

		for(int j = start_x; j<stop_x; j++){
			d[j] = bilinear_interpolation(src, pos_x, pos_y);
			pos_x += step_x;
		}
		pos_y += step_y;
	}
}

template<class S,class D>
void copy_into_image(
		const basic_image_signal<S>& src,
		basic_image_signal<D>& dst,
		double scale
){
//	std::cerr << "x_r = (" << dst.range_x()
//			<< ")\ny_r = (" << dst.range_y() << ")\n";
//	std::cerr << "x_r = (" << src.range_x()
//			<< ")\ny_r = (" << src.range_y() << ")\n";

	image_area copy_area = common_defined_area( src, dst );
//	std::cerr <<  "copy_area: x_r = (" << copy_area.range_x
//			<< ")\n           y_r = (" << copy_area.range_y << ")\n";

	const auto upper_left_point = dst.get_raw_offset({copy_area.range_x.lower_bound(), copy_area.range_y.lower_bound()});
	const auto lower_right_point = dst.get_raw_offset({copy_area.range_x.upper_bound(), copy_area.range_y.upper_bound()});
	const int start_y = upper_left_point.y;
	const int stop_y = lower_right_point.y;
	const int start_x = upper_left_point.x;
	const int stop_x = lower_right_point.x;
//	std::cerr << "x_r = (" << start_x << ", "<< stop_x << ")\ny_r = (" << start_y << ", "<< stop_y << ")\n";

	const auto src_upper_left_point = src.get_raw_offset({copy_area.range_x.lower_bound(), copy_area.range_y.lower_bound()});
	const auto src_lower_right_point = src.get_raw_offset({copy_area.range_x.upper_bound(), copy_area.range_y.upper_bound()});
	const int src_start_y = src_upper_left_point.y;
	const int src_stop_y = src_lower_right_point.y;
	const int src_start_x = src_upper_left_point.x;
	const int src_stop_x = src_lower_right_point.x;
//	std::cerr << "x_r = (" << src_start_x << ", "<< src_stop_x
//			<< ")\ny_r = (" << src_start_y << ", "<< src_stop_y << ")\n";

	image_area::position_type pos_x = src_start_x, pos_y = src_start_y;
	const double step_x = static_cast<double>(src_stop_x - src_start_x) / static_cast<double>(stop_x - start_x);
	const double step_y = static_cast<double>(src_stop_y - src_start_y) / static_cast<double>(stop_y - start_y);

	for(int i = start_y; i<=stop_y; i++){
		D* d = dst[i];
		pos_x = src_start_x;

		for(int j = start_x; j<=stop_x; j++){
			d[j] = std::max<S>( 0.0, scale * bilinear_interpolation(src, pos_x, pos_y));
			pos_x += step_x;
		}
		pos_y += step_y;
	}
}

template<class S,class D>
void copy_into_image(
		const basic_image_signal<S>& src,
		basic_image_signal<D>& dst,
		const image_area& dstArea,
		const image_area& srcArea,
		double scale
){
	const int start_y = dstArea.range_y.lower_bound();
	const int stop_y = dstArea.range_y.upper_bound();
	const int start_x = dstArea.range_x.lower_bound();
	const int stop_x = dstArea.range_y.upper_bound();

	const int src_start_y = srcArea.range_y.lower_bound() > 0 ? srcArea.range_y.lower_bound() : 0;
	const int src_stop_y = srcArea.range_y.upper_bound() < src.raw_height() - 1 ? srcArea.range_y.upper_bound() : src.raw_height() - 1;
	const int src_start_x = srcArea.range_x.lower_bound() > 0 ? srcArea.range_x.lower_bound() : 0;
	const int src_stop_x = srcArea.range_x.upper_bound() < src.raw_width() - 1 ? srcArea.range_x.upper_bound() : src.raw_width() - 1;

	image_area::position_type pos_x = src_start_x, pos_y = src_start_y;
	const double step_x = static_cast<double>(src_stop_x - src_start_x) / static_cast<double>(stop_x - start_x);
	const double step_y = static_cast<double>(src_stop_y - src_start_y) / static_cast<double>(stop_y - start_y);

	for(int i = start_y; i<stop_y; i++){
		D* d = dst[i];
		pos_x = src_start_x;

		for(int j = start_x; j<stop_x; j++){
			d[j] = std::max<S>( 0.0, scale*bilinear_interpolation(src, pos_x, pos_y));
			pos_x += step_x;
		}
		pos_y += step_y;
	}
}


template< class S, class D >
void copy_into_image_scale_offset(
		const basic_image_signal<S>& src,
		basic_image_signal<D>& dst,
		double scale = 1,
		double offset = 0
){

	image_area copy_area = common_defined_area( src, dst );
//	std::cerr <<  "copy_area: x_r = (" << copy_area.range_x
//			<< ")\n           y_r = (" << copy_area.range_y << ")\n";

	const auto upper_left_point = dst.get_raw_offset({copy_area.range_x.lower_bound(), copy_area.range_y.lower_bound()});
	const auto lower_right_point = dst.get_raw_offset({copy_area.range_x.upper_bound(), copy_area.range_y.upper_bound()});
	const int start_y = upper_left_point.y;
	const int stop_y = lower_right_point.y;
	const int start_x = upper_left_point.x;
	const int stop_x = lower_right_point.x;
//	std::cerr << "x_r = (" << start_x << ", "<< stop_x << ")\ny_r = (" << start_y << ", "<< stop_y << ")\n";

	const auto src_upper_left_point = src.get_raw_offset({copy_area.range_x.lower_bound(), copy_area.range_y.lower_bound()});
	const auto src_lower_right_point = src.get_raw_offset({copy_area.range_x.upper_bound(), copy_area.range_y.upper_bound()});
	const int src_start_y = src_upper_left_point.y;
	const int src_stop_y = src_lower_right_point.y;
	const int src_start_x = src_upper_left_point.x;
	const int src_stop_x = src_lower_right_point.x;
//	std::cerr << "x_r = (" << src_start_x << ", "<< src_stop_x
//			<< ")\ny_r = (" << src_start_y << ", "<< src_stop_y << ")\n";

	image_area::position_type pos_x = src_start_x, pos_y = src_start_y;
	const double step_x = static_cast<double>(src_stop_x - src_start_x) / static_cast<double>(stop_x - start_x);
	const double step_y = static_cast<double>(src_stop_y - src_start_y) / static_cast<double>(stop_y - start_y);


	for(int i = start_y; i<stop_y; i++){
		D* d = dst[i];
		pos_x = src_start_x;

		for(int j = start_x; j<stop_x; j++){
			d[j] = std::max<S>(0.0,offset + scale*bilinear_interpolation(src, pos_x, pos_y));
			pos_x += step_x;
		}
		pos_y += step_y;
	}
}


template< class S, class D >
void copy_into_image_scale_offset(
		const basic_image_signal<S>& src,
		basic_image_signal<D>& dst,
		const image_area& dstArea,
		const image_area& srcArea,
		double scale = 1,
		double offset = 0
){
	const int start_y = dstArea.range_y.lower_bound();
	const int stop_y = dstArea.range_y.upper_bound();
	const int start_x = dstArea.range_x.lower_bound();
	const int stop_x = dstArea.range_y.upper_bound();

	const int src_start_y = srcArea.range_y.lower_bound() > 0 ? srcArea.range_y.lower_bound() : 0;
	const int src_stop_y = srcArea.range_y.upper_bound() < src.raw_height() - 1 ? srcArea.range_y.upper_bound() : src.raw_height() - 1;
	const int src_start_x = srcArea.range_x.lower_bound() > 0 ? srcArea.range_x.lower_bound() : 0;
	const int src_stop_x = srcArea.range_x.upper_bound() < src.raw_width() - 1 ? srcArea.range_x.upper_bound() : src.raw_width() - 1;

	image_area::position_type pos_x = src_start_x, pos_y = src_start_y;
	const image_length step_x = static_cast<double>(src_stop_x - src_start_x) / static_cast<double>(stop_x - start_x);
	const image_length step_y = static_cast<double>(src_stop_y - src_start_y) / static_cast<double>(stop_y - start_y);
//	cerr<<"src points x: "<< src_start_x <<" - " << src_stop_x << ",  y: "<< src_start_x <<" - " << src_stop_x <<"\n";
//	cerr<<"dst points x: "<< start_x <<" - " << stop_x << ",  y: "<< start_x <<" - " << stop_x <<"\n";
//	cerr<<"steps: "<<step_x<<", "<<step_y<<"\n";

	for(int i = start_y; i<stop_y; i++){
		D* d = dst[i];
		pos_x = src_start_x;

		for(int j = start_x; j<stop_x; j++){
			d[j] = std::max<S>(0.0,offset + scale*bilinear_interpolation(src, pos_x, pos_y));
			pos_x += step_x;
		}
		pos_y += step_y;
	}
}

}

#endif /* IMAGE_SIGNAL_UTILITY_FUNCTIONS_H_ */




