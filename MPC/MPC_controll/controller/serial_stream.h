#ifndef __SERIAL_STREAM__
#define __SERIAL_STREAM__
#include <iostream>
#include <boost/asio/serial_port.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/iostreams/chain.hpp>
#include <boost/iostreams/stream.hpp>
#include <iostream>
#include <vector>
#include <functional>
#include <fcntl.h>
namespace lacus{
	class serial_device { 
	public: 
		typedef char char_type; 
		typedef boost::iostreams::bidirectional_device_tag category; 
		serial_device(boost::asio::serial_port& port); 
		std::streamsize read(char* buf, std::streamsize n) ;
		std::streamsize write(const char* buf, std::streamsize n) ;
	private: 
		boost::asio::serial_port& serial_port; 
	};

	class serial_stream: public boost::iostreams::stream<serial_device>{
		boost::asio::io_service io;	
		boost::asio::serial_port serial_port;
	public:
		typedef boost::iostreams::stream<serial_device> base_t;
		serial_stream(
				const std::string& device_name,
				int baudrate=115200,
				int character_size=8, 
				const boost::asio::serial_port_base::stop_bits::type& stop_bits=boost::asio::serial_port_base::stop_bits::one,
				const boost::asio::serial_port_base::parity::type& parity=boost::asio::serial_port_base::parity::none,
				const boost::asio::serial_port_base::flow_control::type& flow_control=boost::asio::serial_port_base::flow_control::none
		);
		template<typename Q> void set_option(const Q& option){
			serial_port.set_option(option);
		};
		int tcflush(int type=TCIOFLUSH);
		boost::asio::serial_port::native_handle_type native_handle();
	};
};
#endif