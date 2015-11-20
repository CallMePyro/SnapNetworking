#include "connection_session.h"

void connection_session::start()
{
	_connected_clients.join( shared_from_this() );
	read_header();
}

void connection_session::deliver( const message & msg )
{
	if( msg.get_id() != _id )
	{
		bool write_in_progress = !_msg_queue.empty();
		_msg_queue.push_back( msg );
		if( !write_in_progress ) //if it was empty before we added our message to the queue then process the message. 
		//otherwise the current message in queue will process the next one
		{
			write();
		}
	}
}

void connection_session::read_header()
{
	async_read( _socket, buffer( _cur_msg.data(), message::header_length ), [this]( error_code ec, size_t len )
	{
		if( !ec && _cur_msg.decode_header() )
		{
			read_id();
		}
		else
		{
			_connected_clients.leave( shared_from_this() );
		}
	} );
}

void connection_session::read_id()
{
	async_read( _socket, buffer( _cur_msg.id(), message::id_length ), [this]( error_code ec, size_t len )
	{
		if( !ec && _cur_msg.decode_id() )
		{
			read_body();
		}
		else
		{
			_connected_clients.leave( shared_from_this() );
		}
	} );
}

void connection_session::read_body()
{
	async_read( _socket, buffer( _cur_msg.body(), _cur_msg.body_length() ), [this]( error_code ec, size_t len )
	{
		if( !ec )
		{
			read_username();
		}
		else
		{
			_connected_clients.leave( shared_from_this() );
		}
	} );
}

void connection_session::read_username()
{
	async_read( _socket, buffer( _cur_msg.username(), message::username_length ), [this]( error_code ec, size_t len )
	{
		if( !ec && _cur_msg.decode_username() )
		{
			_connected_clients.deliver( _cur_msg );

			parse_message();
			read_header();
		}
		else
		{
			_connected_clients.leave( shared_from_this() );
		}
	} );
}

void connection_session::parse_message()
{
	string body( _cur_msg.body(), _cur_msg.body_length() );

	if( body != "" )
	{
		cout << _cur_msg.get_username() << ": " << body << '\n';
	}
}

void connection_session::write()
{
	async_write( _socket, buffer( _msg_queue.front().data(), _msg_queue.front().total_length( false ) ), [this]( error_code ec, size_t length )
	{
		if( !ec )
		{
			_msg_queue.pop_front();
			if( !_msg_queue.empty() )
			{
				write();
			}
		}
		else
		{
			_connected_clients.leave( shared_from_this() );
		}
	} );
}