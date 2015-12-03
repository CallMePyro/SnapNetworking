#pragma once

#include <string>
using std::string;
#include <iostream>
#include "sql_singleton.h"

#include "cryptopp563\hex.h"
#include "cryptopp563/sha.h"
#include "cryptopp563/base64.h"

class auth
{
public:
	static bool valid_login( const string & user, const string & pass );
	static string hash_salt_password(const string & user, const string & pass );
private:
};