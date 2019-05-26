#pragma once
#ifndef DATABASE
#define DATABASE

#include <vector>
#include <string>
#include <iterator>

#include "Table.h"

namespace dbms {
	class Database {
		//! dbms::Database class
		/** \class Database
		* All the attributes of this class are public.
		* Each instance of Database class contains following:
		*	1) databaseName : string
		*	2) tables : std::vector<Table*>
		*
		* Constructor of this class accepts databaseName : string as an input.
		* Output stream operator << is overloaded to output tables vector.
		*/
		public:
			std::string databaseName;
			std::vector<Table*> tables;

			Database(std::string databaseName);
			~Database() = default;

			friend std::ostream &operator<<(std::ostream &output, const Database *database);
	};
}
#endif