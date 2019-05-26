#pragma once
#ifndef REGISTRATOR
#define REGISTRATOR

#include "Logger.h"

namespace dbms {
	class Registrator : public Logger
	{
	public:
		Registrator(std::string userName, std::string password);
		~Registrator() = default;

		void registerUser(std::string userName, std::string password);
	};
}
#endif

