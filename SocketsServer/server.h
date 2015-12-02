#pragma once

#include "client.h"
#include "auth.h"
#include <boost/asio.hpp>
using boost::asio::ip::address;
using boost::asio::io_service;
using boost::asio::ip::tcp; //tcp master race
using boost::system::error_code;
using boost::asio::buffer;

#include <cppdb\frontend.h>

#include "db_conn_str.h"

#include <vector>
using std::vector;
#include <string>
using std::string;

class server
{
public:
	server( io_service & io_service, int port, const db_conn_str & con_str );

private:
	void accept();
	void send_id();
	void send_fail_auth();

	cppdb::session _sql_server;
	tcp::acceptor _acceptor;
	tcp::socket _socket;
	client_handler _room;
	int cur_id = 1;
};