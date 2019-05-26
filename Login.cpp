#include "pch.h"
#include "Login.h"


dbms::Login::Login(std::string userName, std::string password)
{
	if (!isUser(userName, password)) {
		throw std::invalid_argument("\n\t*Invalid password or Username!\n");
	}
}

bool dbms::Login::isUser(std::string userName, std::string password)
{
	std::string encryptedPassword = encryptPassword(password);
	for (auto& x : this->userCredentials) {
		if (x.first == userName && x.second == encryptedPassword) {
			return true;
		}
	}
	return false;
}
