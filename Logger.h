#pragma once
#ifndef LOGGER
#define LOGGER

#include "FileIOHelper.h"
#include <map>
#include <string>

namespace dbms {
	/*! \class Logger
	 *	\brief Base class for Login and Registrator classes.
	 */
	class Logger {

	private:
		const std::string file = "users_data.txt";
	protected:
		std::map<std::string, std::string> userCredentials;
	public:
		/*! \Constructor Logger()
		 * Uses FiliOIHelper to read all the Users usernames and passwords from the "users_data.txt".
		 * Then, parses the data and saves usernames and passwords into the map - userCredentials.
		 */
		Logger();

		/*! /Destructor ~Logger()
		 * Uses FileIOHelper to override the "users_data.txt" with the current data from map - userCredentials.
		 */
		~Logger();
		
		// Can accept a string password and return a sha-256 encrypted string using picosha256.h 
		virtual std::string encryptPassword(std::string password)
		{
			std::string encryptedPassword = "";
			picosha2::hash256_one_by_one hasher;

			hasher.init(); //reset hasher state
			hasher.process(password.begin(), password.end());
			hasher.finish();

			picosha2::get_hash_hex_string(hasher, encryptedPassword);

			return encryptedPassword;
		}

		// returns a string representation of the member map called userCredentials (for debug only)
		virtual std::string to_string() const;
		
		// returns this->to_string() to the ostream (for debug only)
		friend std::ostream &operator<<(std::ostream &output, const Logger *logger);
	};
}

#endif//!LOGGER
