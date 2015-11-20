#include "message.h"

message::message(): _body_length( 0 ), _username( "" ) {}

message::message( const char * buf )
{
	write( buf );
}

const char * message::data() const
{
	return _data;
}

char * message::data()
{
	return _data;
}

const char * message::id() const
{
	return _data + header_length;
}

char * message::id()
{
	return _data + header_length;
}

const char * message::username() const
{
	return _data + header_length + id_length + _body_length;
}

char * message::username()
{
	return _data + header_length + id_length + _body_length;
}

const char* message::body() const
{
	return _data + header_length + id_length;
}

char* message::body()
{
	return _data + header_length + id_length;
}

size_t message::total_length( bool username_attached ) const
{
	if( username_attached )
		return header_length + id_length + _body_length + username_length;
	else
		return header_length + id_length + _body_length;
}

size_t message::body_length() const
{
	return _body_length;
}

void message::body_length( size_t new_length )
{
	_body_length = new_length > max_body_length ? max_body_length : new_length;
}

void message::write( const char * buf )
{
	size_t len = strlen( buf );
	body_length( len );
	memcpy( body(), buf, _body_length );
	encode_header();
}

/*
bool message::decode_header()
{
	char header[header_length + 1] = { '\0' };
	strncat( header, _data, header_length );

	try
	{
		_body_length = atoi( header );
	}
	catch( ... )
	{
		_body_length = 0;
		return false;
	}

	if( _body_length > max_body_length )
	{
		_body_length = 0;
		return false;
	}
	return true;
}
*/

//when we decode it just take the bytes, and convert them to an int. gotta love pointer gymnastics.
bool message::decode_header()
{
	_body_length = *(int *)_data;
	return true;
}

/*
void message::encode_header()
{
	char buf[header_length + 1] = { '\0' };
	sprintf( buf, "%4d", static_cast<int>( _body_length ) );
	memcpy( _data, buf, header_length );
}
*/

//I know this looks like hell. Basically just write 8 bits at a time to the buffer
void message::encode_header()
{
	_data[3] = ( _body_length >> 24 ) & 0xFF;
	_data[2] = ( _body_length >> 16 ) & 0xFF;
	_data[1] = ( _body_length >> 8 ) & 0xFF;
	_data[0] = _body_length & 0xFF ;
}

/*
bool message::decode_id()
{
	char buf[id_length + 1] = { '\0' };
	strncat( buf, _data + header_length, id_length );
	try
	{
		_id = atoi( buf );
		return true;
	}
	catch( ... )
	{
		_id = -1;
		return false;
	}
}
*/

bool message::decode_id()
{
	_id = *(int*)id();
	return true;
}

/*
void message::encode_id( int id )
{
	_id = id;
	char buf[id_length + 1] = { '\0' };
	sprintf( buf, "%4d", _id );
	memcpy( this->id(), buf, id_length );
}
*/

void message::encode_id( int id )
{
	this->id()[3] = ( id >> 24 ) & 0xFF;
	this->id()[2] = ( id >> 16 ) & 0xFF;
	this->id()[1] = ( id >> 8 ) & 0xFF;
	this->id()[0] = id & 0xFF;
}

bool message::decode_username()
{
	char buf[message::username_length] = { '\0' };
	for( unsigned i = 0; i < int( message::username_length ); ++i )
	{
		buf[i] = username()[i];
	}
	_username = buf;
	return true;
}

void message::encode_username( string username )
{
	memset( this->username(), '\0', message::username_length );
	for( unsigned i = 0; i <= (int)message::username_length && i < username.length(); ++i )
		this->username()[i] = username[i];
}
 
int message::get_id() const
{
	return _id;
}

string message::get_username() const
{
	return _username;
}