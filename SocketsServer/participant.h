#pragma once
#include "../SharedCode/message.h"

//I know it's tempting to remove this class.
//Don't. The inheritance is needed to keep the shared pointers happy.
class participant
{
public:
	virtual void deliver( const message & msg ) = 0;
};