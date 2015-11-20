#pragma once
#include "../SharedCode/message.h"

class participant
{
public:
	virtual ~participant() {};
	virtual void deliver( const message & msg ) = 0;
};