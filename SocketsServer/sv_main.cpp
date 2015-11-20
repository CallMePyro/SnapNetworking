#include <boost/asio.hpp>
#include "client.h"
#include "server.h"

#include <iostream>
using std::cin;
using std::cout;
#include <string>
using std::string;

using boost::asio::ip::address;
using boost::asio::io_service;
using boost::asio::ip::tcp; //tcp master race

int main()
{
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

	std::cin.get();
	return 0;
}