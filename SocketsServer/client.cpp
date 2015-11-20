#include "client_handler.h"

void client_handler::join( shared_ptr<participant> _cl )
{
	participants_.insert( _cl );
	for( auto msg : recent_msgs_ )
		_cl->deliver( msg );
}

void client_handler::leave( shared_ptr<participant> _cl )
{
	participants_.erase( _cl );
}

void client_handler::deliver( const message & msg )
{
	if( msg.get_id() > 0 ) //only deliver messages with valid id's
	{
		recent_msgs_.push_back( msg );

		if( MAX_MESSAGES > 0 )
			while( recent_msgs_.size() > MAX_MESSAGES )
				recent_msgs_.pop_front();

		for( auto participant : participants_ )
			participant->deliver( msg );
	}
}