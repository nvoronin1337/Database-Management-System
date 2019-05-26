
#pragma once

#ifndef LOGIN
#define LOGIN


#include "Logger.h"

namespace dbms {
	class Login : public Logger
	{
	public:
		Login() = default;
		// Constructor throws std::invalid_argument exception if there is no user with such credentials
		Login(std::string userName, std::string password);
		~Login() = default;

		bool isUser(std::string userName, std::string password);
	};
}

#endif

