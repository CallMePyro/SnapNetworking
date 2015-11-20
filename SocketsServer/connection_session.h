#pragma once
#include "../SharedCode/message.h"
#include "participant.h"
#include "client_handler.h"
#include <boost/asio.hpp>
using boost::asio::ip::tcp;
using boost::system::error_code;
using boost::asio::buffer;
using boost::asio::async_read;
using boost::asio::async_write;

#include <iostream>
using std::cout;
#include <string>
using std::string;
#include <deque>
using std::deque;
#include <memory>
using std::shared_ptr;
#include <set>
using std::set;

class connection_session: public participant, public std::enable_shared_from_this<connection_session>
{
public:
	//not gunna bother putting only the initialization list in a seperate file. this is clearer anyways
	connection_session( tcp::socket socket, client_handler & room, int id ): _socket( std::move( socket ) ), _connected_clients( room ), _id(id) {}

	void start();
	void deliver( const message & msg ) override;

private:

	void read_header();
	void read_id();
	void read_body();
	void read_username();
	void write();
	void parse_message();

	tcp::socket _socket;
	client_handler& _connected_clients;
	message _cur_msg;
	deque<message> _msg_queue;
	int _id;
};