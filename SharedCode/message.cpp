#include "message.h"

message::message(): _body_length( 0 ), _username( "" ) {}

message::message( const char * buf )
{
	write( buf );
}

char * message::data()
{
	return _data;
}

char * message::id()
{
	return _data + header_length;
}

char* message::body()
{
	return _data + header_length + id_length;
}

char * message::username()
{
	return _data + header_length + id_length + _body_length;
}

size_t message::total_length() const
{
#ifdef CLIENT
		return header_length + id_length + _body_length + username_length;
#elif SERVER
		return header_length + id_length + _body_length;
#endif
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
	body_length( strlen( buf ) );
	memcpy( body(), buf, _body_length );
	encode_header();
}

//when we decode it just take the bytes and convert them to an int. gotta love pointer gymnastics.
bool message::decode_header()
{
	_body_length = *(int *)_data;
	return _body_length >= 0;
}

//I know this looks like hell. Basically just write 8 bits at a time to the buffer
void message::encode_header()
{
	_data[3] = ( _body_length >> 24 ) & 0xFF;
	_data[2] = ( _body_length >> 16 ) & 0xFF;
	_data[1] = ( _body_length >> 8 ) & 0xFF;
	_data[0] = _body_length & 0xFF ;
}

void message::decode_id()
{
	_id = *(int*)id();
}

void message::encode_id( int id )
{
	this->id()[3] = ( id >> 24 ) & 0xFF;
	this->id()[2] = ( id >> 16 ) & 0xFF;
	this->id()[1] = ( id >> 8 ) & 0xFF;
	this->id()[0] = id & 0xFF;
}

void message::decode_username()
{
	char buf[message::username_length] = { '\0' };
	for( unsigned i = 0; i < int( message::username_length ); ++i )
		buf[i] = username()[i];
	_username = buf;
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