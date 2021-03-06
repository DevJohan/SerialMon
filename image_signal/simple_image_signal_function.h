/*
 * simple_image_signal_function.h
 *
 *  Created on: 27 jun 2013
 *      Author: johan
 */

#ifndef SIMPLE_IMAGE_SIGNAL_FUNCTION_H_
#define SIMPLE_IMAGE_SIGNAL_FUNCTION_H_

#include "simple_image_signal.h"

namespace _basic_image_signal {

template <typename Ta,typename Tb>
image_area common_defined_area( const basic_image_signal<Ta>& image_a, const basic_image_signal<Tb>& image_b ){

	image_area::position_type x_min =
			max( image_a.range_x().lower_bound(), image_b.range_x().lower_bound() );
	image_area::position_type x_max =
			min( image_a.range_x().upper_bound(), image_b.range_x().upper_bound() );

	image_area::position_type y_min =
			max( image_a.range_y().lower_bound(), image_b.range_y().lower_bound()  );
	image_area::position_type y_max =
			min( image_a.range_y().upper_bound(), image_b.range_y().upper_bound()  );

	return image_area{{x_min,x_max},{y_min,y_max}};
}

template <typename T>
T image_signal_max(const basic_image_signal<T> & image){
	if(image.raw_height() * image.raw_width() <= 0 )
		throw 0;

	T max_value = image[0][0];
	for(int i = 0;i<image.raw_height();i++){
		const T* row_data = image[i];
		for(int j = 0;j<image.raw_width();j++){
			if( row_data[j] > max_value )
				max_value = row_data[j];

		}
	}
	return max_value;
}

};

#endif /* SIMPLE_IMAGE_SIGNAL_FUNCTION_H_ */
