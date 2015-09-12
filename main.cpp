///*
// * main.cpp
// *
// *  Created on: 1 feb 2015
// *      Author: johan
// */
//#include <iostream>
//
//int main(){
//	boost::asio::io_service io;
//	boost::asio::serial_port port(io, "/dev/ttyACM0");
//	port.set_option( boost::asio::serial_port::baud_rate( 115200 ) );
//	port.set_option( boost::asio::serial_port::flow_control( boost::asio::serial_port::flow_control::none ) );
//	port.set_option( boost::asio::serial_port::parity( boost::asio::serial_port::parity::none ) );
//	port.set_option( boost::asio::serial_port::stop_bits( boost::asio::serial_port::stop_bits::one ) );
//	port.set_option( boost::asio::serial_port::character_size( 8 ) );
////	port.open();
//
//	while(true){
////		if( std::cin.gcount() > 0 ){
////			port.write_some( boost::asio::buffer())
////		}
//		char c;
//		port.read_some(boost::asio::buffer(&c,1));
//		std::cout << c;
//	}
//	return 0;
//}
//

// wxWidgets "Hello world" Program
// For compilers that support precompilation, includes "wx/wx.h".
#include <iostream>
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include "UM2ListenerFrame.h"
#include <boost/asio/serial_port.hpp>
#include <boost/asio/serial_port_service.hpp>
#include <thread>

class UM2ListenerApp;

class serial_com{
private:
	serial_com& operator=(const serial_com&);
	serial_com(const serial_com&);
public:
	static constexpr size_t MAX_READ_SIZE = 32;
	void issue_read(){
		if(!is_shutting_down){
			port.async_read_some(
					boost::asio::buffer(&read_buffer,MAX_READ_SIZE),
					[this](
							const boost::system::error_code& ec,
							std::size_t bytes_transferred
					){ handle_read(ec,bytes_transferred); });
		}
	}
	void handle_read(
			const boost::system::error_code& ec,
			std::size_t bytes_transferred);
	void set_data_handler( UM2ListenerApp* handler ){
		data_handler = handler;
	}
	serial_com( UM2ListenerApp* handler = nullptr ):
		io(),
		port(io, "/dev/ttyACM0"),
		is_shutting_down(false),
		io_thread(nullptr),
		data_handler( handler )
{
		port.set_option( boost::asio::serial_port::baud_rate( 115200 ) );
		port.set_option( boost::asio::serial_port::flow_control( boost::asio::serial_port::flow_control::none ) );
		port.set_option( boost::asio::serial_port::parity( boost::asio::serial_port::parity::none ) );
		port.set_option( boost::asio::serial_port::stop_bits( boost::asio::serial_port::stop_bits::one ) );
		port.set_option( boost::asio::serial_port::character_size( 8 ) );

		io.post( [this](){issue_read();} );
		io_thread = new std::thread( [this](){ io.run(); } );

}
	~serial_com(){
		is_shutting_down = true;
		io_thread->join();
	}
private:
	boost::asio::io_service io;
	boost::asio::serial_port port;

	bool is_shutting_down;
	std::thread* io_thread;
	char read_buffer[MAX_READ_SIZE];

	UM2ListenerApp* data_handler;
};


class UM2ListenerApp: public wxApp
{
public:
	virtual bool OnInit() override;
	virtual void CleanUp() override;

	void handle_data( const std::string& str );

private:
	UM2ListenerFrame* frame;
	serial_com* p_service;
};

wxIMPLEMENT_APP(UM2ListenerApp);

void UM2ListenerApp::handle_data( const std::string& str ){
	frame->post_to_log( str );
}

bool UM2ListenerApp::OnInit()
{
	frame = new UM2ListenerFrame( "Ultimaker2 Listener Application", wxPoint(50, 50), wxSize(450, 340) );
	frame->Show( true );

//	p_service = new serial_com( this );

	return true;
}

void UM2ListenerApp::CleanUp()
{
	if(p_service){
		delete p_service;
	}
}

void serial_com::handle_read(
		const boost::system::error_code& ec,
		std::size_t bytes_transferred){
	if(!ec){
//		std::cout << std::string(read_buffer,bytes_transferred) << " " << bytes_transferred << "\n";
		if(data_handler)
			data_handler->handle_data(std::string(read_buffer,bytes_transferred));
		issue_read();
	}else if(data_handler){
		data_handler->handle_data("\n*\n*Error in serial read\n*\n*\n");
	}
}
