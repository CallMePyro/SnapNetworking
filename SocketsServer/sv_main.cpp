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
#include "sql_singleton.h"

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

		db_conn_str con_str( "aura.students.cset.oit.edu", "ryan_williams", "ryan_williams", "Dr8g0nk1n7!" );
		sql_singleton::instance().open( con_str.get() );
		cout << "Sucessfully connected to database.\n";

		io_service io_service;

		server server( io_service, port );

		cout << "Server is listening on port " << port << "...\n";
		io_service.run();
	}
	catch( cppdb::cppdb_error & e ) //if it's a database error we'll catch that first
	{
		cout << "Database Exception: " << e.what() << '\n';
	}
	catch( std::exception & e ) //otherwise who knows what went wrong, jesus.
	{
		cout << "General Exception: " << e.what() << "\n";
	}

	system( "pause" );
	return 0;
}