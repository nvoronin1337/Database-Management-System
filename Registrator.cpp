#include "pch.h"
#include "Registrator.h"

dbms::Registrator::Registrator(std::string userName, std::string password)
{
	registerUser(userName, password);
}

void dbms::Registrator::registerUser(std::string userName, std::string password)
{
	std::string encryptedPassword = encryptPassword(password);
	this->userCredentials[userName] = encryptedPassword;
}
