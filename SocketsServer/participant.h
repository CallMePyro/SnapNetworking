#pragma once
#include "../SharedCode/message.h"

class participant
{
public:
	virtual void deliver( const message & msg ) = 0;
};