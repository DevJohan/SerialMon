/*
 * Utility.h
 *
 *  Created on: 30 sep 2010
 *      Author: Johan
 */

#ifndef GENERAL_SIGNAL_UTILITY_H_
#define GENERAL_SIGNAL_UTILITY_H_

#include <cmath>
#include <iostream>
using std::cerr;


namespace _basic_image_signal{

template<class T> inline T array_max(const T* array, const int size){
	T current_max = array[0];
	for(int i = 1; i< size; i++){
		if(current_max < array[i])
			current_max = array[i];
	}
	return current_max;
}


template<class T> inline T array_min(const T* array, const int size){
	T current_min = array[0];
	for(int i = 1; i< size; i++){
		if(array[i] < current_min)
			current_min = array[i];
	}
	return current_min;
}


template<class T> inline void array_max_min(T* max, T* min, const T* array, const int size){
	T current_max = array[0];
	T current_min = array[0];
	for(int i = 1; i< size; i++){
		if(array[i] < current_min){
			current_min = array[i];
		}else if(current_max < array[i]){
			current_max = array[i];
		}
	}
	*max = current_max;
	*min = current_min;
}

inline double euclidean_remainder(const double numerator, const double denominator){
	const double remainder = std::fmod(numerator, denominator);
	return remainder >= 0 ? remainder : remainder + denominator;
}


/****
 * 	Converts float to int, round to nearest.
 */
inline int f2i(const float value){
	return value >=0 ? value + .5f : value - .5f;
}
inline int f2i_p(const float value){
	return value + .5f;
}



}

#endif /* GENERAL_SIGNAL_UTILITY_H_ */
