#include "client.h"

void client::start()
{
	_connected_clients.join( shared_from_this() );
	read_header();
}

void client::deliver( const message & msg )
{
	if( msg.get_id() != _id )
	{
		bool empty = _msg_queue.empty();
		_msg_queue.push_back( msg );
		if( empty ) //Otherwise the preceeding message in the queue will process this one.
			write();
	}
}

void client::read_header()
{
	async_read( _socket, buffer( _cur_msg.data(), message::header_length ), [this]( error_code ec, size_t len )
	{
		if( !ec && _cur_msg.decode_header() )
			read_id();
		else
			_connected_clients.leave( shared_from_this() );
	} );
}

void client::read_id()
{
	async_read( _socket, buffer( _cur_msg.id(), message::id_length ), [this]( error_code ec, size_t len )
	{
		if( !ec )
		{
			_cur_msg.decode_id();
			read_body();
		}
		else
		{
			_connected_clients.leave( shared_from_this() );
		}
	} );
}

void client::read_body()
{
	async_read( _socket, buffer( _cur_msg.body(), _cur_msg.body_length() ), [this]( error_code ec, size_t len )
	{
		if( !ec )
			read_username();
		else
			_connected_clients.leave( shared_from_this() );
	} );
}

void client::read_username()
{
	async_read( _socket, buffer( _cur_msg.username(), message::username_length ), [this]( error_code ec, size_t len )
	{
		if( !ec )
		{
			_cur_msg.decode_username();
			parse_message();
			read_header();
		}
		else
		{
			_connected_clients.leave( shared_from_this() );
		}
	} );
}

void client::parse_message()
{
	if( _cur_msg.get_id() == _id && _cur_msg.get_username() == _username ) //wow real secure jacob
	{
		string body( _cur_msg.body(), _cur_msg.body_length() );

		if( body != "" )
		{
			_connected_clients.deliver( _cur_msg );
			cout << _cur_msg.get_username() << ": " << body << '\n';
		}
	}
}

void client::write()
{
	async_write( _socket, buffer( _msg_queue.front().data(), _msg_queue.front().total_length() ), [this]( error_code ec, size_t length )
	{
		if( !ec )
		{
			_msg_queue.pop_front();
			if( !_msg_queue.empty() )
				write();
		}
		else
		{
			_connected_clients.leave( shared_from_this() );
		}
	} );
}