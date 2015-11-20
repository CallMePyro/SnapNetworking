#pragma once
#include "../SharedCode/message.h"
#include <boost/asio.hpp>
using boost::asio::io_service;
using boost::asio::ip::tcp;
using boost::system::error_code;
using boost::asio::buffer;
using boost::asio::async_write;
using boost::asio::async_read;
using boost::asio::async_connect;
#include <deque>
using std::deque;
#include <iostream>
using std::cout;
using std::cin;
#include <string>
using std::string;

class connection_session
{
public:
	connection_session( io_service & service, const string & host, const string & port, const string & username, const string & pass );

	void write( const message & msg );
	void close();

private:
	void connect( tcp::resolver::iterator endpoint_iterator, const string & pass );

	void read_header();
	void read_id();
	void read_body();
	void parse_message( const string & message );

	void write();

private:
	io_service & _service;
	tcp::socket _socket;
	message _cur_msg;
	deque<message> _msg_queue;

	int _id;
	string _username;
};