/*
 * data_holder.h
 *
 *  Created on: 5 feb 2015
 *      Author: johan
 */

#ifndef IMAGE_SIGNAL_DATA_HOLDER_H_
#define IMAGE_SIGNAL_DATA_HOLDER_H_


namespace _basic_image_signal {

typedef size_t generic_size_type;

template < typename T >
class data_holder{
public:
	typedef generic_size_type size_type;
	typedef T elem_type;

	size_type _data_size;
	elem_type* _data;

	data_holder( ) :
		_data_size(0),
		_data(nullptr){}

	data_holder( size_type allocation_size ):
		_data_size(allocation_size),
		_data( allocate_data_impl(_data_size) ) { }

	~data_holder( ){
		if( _data != nullptr ) {
			deallocate_data_impl();
		}
	}



	data_holder( const data_holder& original ):
		_data_size( original._data_size ),
		_data( allocate_data_impl(_data_size) )
	{
		data_copy_impl( original );
	}

	data_holder& operator=( const data_holder& original ){
		if( this != &original ){
			validate_size_impl( original._data_size );
			data_copy_impl( original );
		}
		return *this;
	}

	data_holder( data_holder&& original ):
		_data_size( original._data_size ),
		_data( original._data )
	{
		original._data = nullptr;
		original._data_size = 0;
	}

	data_holder& operator=( data_holder&& original ) {
		std::swap( _data_size , original._data_size );
		std::swap( _data, original._data );
		return *this;
	}

	elem_type* start() { return _data; }
	const elem_type* start() const { return _data; }
	elem_type* end() { return _data + allocated_size_impl(); }
	const elem_type* end() const { return _data + allocated_size_impl(); }

	elem_type* data() { return _data; }
	const elem_type* data() const { return _data; }

	size_type size() const { return allocated_size_impl(); }
	size_type capacity() const  { return allocated_size_impl(); }
	void reserve( size_type new_size ){
		validate_size_impl( new_size );
	}


	elem_type& operator[]( const size_type index ){ return _data[index]; }
	const elem_type& operator[]( const size_type index ) const { return _data[index]; }


private:
	void validate_size_impl( const size_type new_data_size ){
		if(_data != nullptr ){
			if( _data_size < new_data_size ){
				deallocate_data_impl();
				_data_size = new_data_size;
				_data = allocate_data_impl( _data_size );
			}
		}else{
			_data_size = new_data_size;
			_data = allocate_data_impl(_data_size);
		}
	}
	void deallocate_data_impl(){
		delete[] _data;
		_data = nullptr;
	}
	size_type allocated_size_impl() const {
		return _data_size;
	}
	static elem_type* allocate_data_impl( const size_type data_size){
		return new elem_type[data_size];
	}
	void data_copy_impl(const data_holder& original){
		for(size_type i = 0; i < original._data_size; i++ ){
			_data[ i ] = original._data[ i ];
		}
	}
};


template < typename T >
class no_copy_data_holder: public data_holder<T>{
public:
	using typename data_holder<T>::size_type;
	no_copy_data_holder( ) : data_holder<T>() {}
	no_copy_data_holder( size_type allocation_size ) : data_holder<T>( allocation_size ) {}
	no_copy_data_holder( const no_copy_data_holder& ) = delete;
	no_copy_data_holder& operator=( const no_copy_data_holder& ) = delete;
	no_copy_data_holder( no_copy_data_holder&& ) = default;
	no_copy_data_holder& operator=( no_copy_data_holder&& ) = default;
};

};

#endif /* IMAGE_SIGNAL_DATA_HOLDER_H_ */
