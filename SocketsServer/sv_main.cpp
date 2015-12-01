#include <boost/asio.hpp>
#include "client.h"
#include "server.h"

#include <cppdb/frontend.h>
#include <cppdb\driver_manager.h>
#include <cppdb\backend.h>

#include <iostream>
using std::cin;
using std::cout;
#include <string>
using std::string;
#include <thread>
using std::thread;

using boost::asio::ip::address;
using boost::asio::io_service;
using boost::asio::ip::tcp; //tcp master race

int main()
{
	/*
	try
	{
		int port;
		cout << "Enter server port: ";
		cin >> port;

		io_service io_service;

		tcp::endpoint endpoint( tcp::v4(), port );
		server server( io_service, endpoint );

		cout << "Server listening on port " << port << "...\n";
		io_service.run();
	}
	catch( std::exception& e )
	{
		std::cerr << "Exception: " << e.what() << "\n";
	}
	//*/
	try
	{
		cppdb::session sql( "odbc:Driver={SQL Server};SERVER=aura.students.cset.oit.edu;DATABASE=ryan_williams;UID=ryan_williams;PWD=Dr8g0nk1n7!" );
		cppdb::result r = sql << "SELECT name FROM SnapProducts";
		while( r.next() )
		{
			string name;
			r.fetch( 0, name );
			cout << name << '\n';
		}


	}
	catch( const cppdb::cppdb_error & e )
	{
		cout << e.what() << '\n';
	}

	system( "pause" );
	return 0;
}