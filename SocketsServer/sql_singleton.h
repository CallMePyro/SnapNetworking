#pragma once
#include <cppdb\frontend.h>
class sql_singleton
{
public:
	static cppdb::session & instance()
	{
		static cppdb::session sv;
		return sv;
	}

private:
	sql_singleton() {}
	sql_singleton( sql_singleton const & copy ) = delete;
	void operator=( sql_singleton const & copy ) = delete;
};