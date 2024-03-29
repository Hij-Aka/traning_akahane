#include "serial_stream.h"
#include <fcntl.h>
#include <termios.h>
using namespace std;
using namespace boost;
using namespace lacus;
using namespace boost::asio;
namespace{
	}
serial_device::serial_device(boost::asio::serial_port& port) : serial_port(port) {};

std::streamsize serial_device::read(char* buf, std::streamsize n) {
	return serial_port.read_some(boost::asio::buffer(buf, n)); 
};
std::streamsize serial_device::write(const char* buf, std::streamsize n){
	return serial_port.write_some(boost::asio::buffer(buf, n)); 
};

serial_stream::serial_stream(
		const string& port_name, 
		int baud, int csize, 
		const serial_port_base::stop_bits::type& sbits, 
		const serial_port_base::parity::type& parity,
		const serial_port_base::flow_control::type& tp
	):io(),serial_port(io,port_name),base_t(serial_port){
	auto &sp=serial_port;
	sp.set_option(serial_port_base::baud_rate(baud));
	sp.set_option(serial_port_base::flow_control(tp));
	sp.set_option(serial_port_base::parity(parity));
	sp.set_option(serial_port_base::stop_bits(sbits));
	sp.set_option(serial_port_base::character_size(csize));
};
int serial_stream::tcflush(int type){
	return ::tcflush(serial_port.lowest_layer().native_handle(),type);
};
boost::asio::serial_port::native_handle_type serial_stream::native_handle(){
	return serial_port.lowest_layer().native_handle();
};