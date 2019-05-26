#pragma once
#ifndef PASSWORD
#define PASSWORD

#include <iostream>
#include <string>
//#include <vector>

namespace dbms {
	class PasswordChecker
	{
	private:
		std::string password;
	public:
		PasswordChecker() = default;
		~PasswordChecker() = default;

		std::pair<bool, std::string> checkPassword(std::string password) {
			if (password.length() < 9) {
				return std::make_pair(false, "Password it not long enough. Need at lease 9 characters.");
			}
			else {
				bool hasNumber = false;
				bool hasUpper = false;
				bool hasLower = false;
				bool hasSymbol = false;
				
				for (char ch : password) {
					// if ch is a number
					if (ch >= 48 && ch <= 57) {
						hasNumber = true;
						std::cout << "Has num: " << ch << std::endl;
					}
					// else if ch is an upper case english letter
					else if(ch >= 65 && ch <= 90) {
						hasUpper = true;
						std::cout << "Has upper: " << ch << std::endl;
					}
					//else if ch is a lower case inglish letter
					else if (ch >= 161 && ch <= 186) {
						hasLower = true;
						std::cout << "Has lower: " << ch << std::endl;
					}
					// else if ch is one of these common symbols: {'!','"'...}
					else if (ch >= 33 && ch <= 47) {
						hasSymbol = true;
						std::cout << "Has symbol: " << ch << std::endl;
					}
					else {
						return std::make_pair(false, "Unknown character: " + ch);
					}
				}
				if (hasNumber && hasUpper && hasLower && hasSymbol) {
					return std::make_pair(true, "Strong Password.");
				}
				else {
					return std::make_pair(false, "Password is not strong enough.");
				}

			}
		}
	};
}
#endif