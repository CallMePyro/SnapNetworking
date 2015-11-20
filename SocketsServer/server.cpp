#include "server.h"

server::server( io_service & service, const tcp::endpoint & endpoint ): _acceptor( service, endpoint ), _socket( service )
{
	accept();
}

void server::accept()
{
	_acceptor.async_accept( _socket, [this]( error_code ec )
	{
		if( !ec )
		{
			char buf[message::max_body_length] = { '\0' };
			_socket.read_some( buffer( buf, message::max_body_length ) );

			string user = strtok( buf, " " );
			string pass = strtok( nullptr, " " );
			//string ip = _socket.remote_endpoint().address().to_string();

			if( auth::valid_login( user, pass ) )
			{
				send_id(); //when a client connects initially, send them their ID
				std::make_shared<connection_session>( std::move( _socket ), _room, cur_id - 1 )->start(); //set the socket in an asynchronous waiting state

				cout << user << " (id: " << cur_id - 1 << ") has connected to the server\n";
			}
			else
				send_fail_auth();

		}
		accept();
	} );
}

void server::send_id()
{
	message id_message( "valid_login" );
	id_message.encode_id( cur_id++ );

	_socket.write_some( buffer( id_message.data(), id_message.total_length( false ) ) );
}

void server::send_fail_auth()
{

}