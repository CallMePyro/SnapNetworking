#pragma region Includes
#include <boost/asio.hpp>
using boost::asio::ip::address;
using boost::asio::io_service;
using boost::asio::ip::tcp; //tcp master race

#include "client.h"
#include "server.h"

#include <cppdb/frontend.h>
#include <cppdb\driver_manager.h>
#include <cppdb\backend.h>
#include "db_conn_str.h"

#include <iostream>
using std::cin;
using std::cout;
#include <string>
using std::string;
#include <thread>
using std::thread;
#pragma endregion Includes


int main()
{
	try
	{
		int port;
		cout << "Enter server port: ";
		cin >> port;

		io_service io_service;

		db_conn_str con_str( "aura.students.cset.oit.edu", "ryan_williams", "ryan_williams", "Dr8g0nk1n7!" );
		server server( io_service, port, con_str );

		thread t( [port]() { cout << "Server successfully connected to database and is listening on port " << port << "...\n"; } ); //it's a threaded 'cout' statement. We're in the future now boys
		io_service.run();
	}
	catch( cppdb::cppdb_error & e ) //if it's a database error we'll catch that first
	{
		cout << "Exception: " << e.what() << '\n';
	}
	catch( std::exception & e ) //otherwise who knows what went wrong, jesus.
	{
		cout << "Exception: " << e.what() << "\n";
	}

	system( "pause" );
	return 0;
}