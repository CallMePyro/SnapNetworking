#pragma once
#include "participant.h"
#include "../SharedCode/message.h"

#include <memory>
using std::shared_ptr;
#include <set>
using std::set;
#include <deque>
using std::deque;

class client_handler
{
public:
	void join( shared_ptr<participant> participant );
	void leave( shared_ptr<participant> participant );
	void deliver( const message & msg );

private:
	set<shared_ptr<participant>> participants_;
	deque<message> recent_msgs_;

	static const int MAX_MESSAGES = 50; //if this value less than 1, there are unlimited messages allowed in the queue.
};