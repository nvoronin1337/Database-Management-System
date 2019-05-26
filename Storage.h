#pragma once
#ifndef STORAGE
#define STORAGE

#include <vector>
#include <iterator>

#include "Database.h"

namespace dbms {
	class Storage {
		//! dbms::Storage class.

		/** \class Storage dbms::Storage class represents the collection of all the users' databases,
		*		tables, and records.
		*
		*	All members of the dbms::Storage class are public.
		*	Storage contains only one member attribute which is a vector pointers to all
		*   the databases.
		*
		*  The constructor and the Destructor of this class are default.
		*
		*	std::string to_string() method returns a string representation of the storage.
		*	Such representations includes all the databases, their tables, and tables' records.
		*
		*  Output stream operator "<<" is overloaded to output a string representation of the
		*  storage using the dbms::Storage::to_string() method.
		*
		*	This class contains three simillar methods for obtaining the database's, table's, or records's
		*  index in the vector which contains this storage's databases, tables, or records.
		*/

		public:
			std::vector<Database*> databases;
			
			Storage() = default;
			~Storage();

			/**
			* A friend member. Overloaded output stream <<.
			* @param  std::ostream object like std::cout or std::ofstream
			* @param  Storage* a pointer to the dbms::Storage object
			*/
			friend std::ostream &operator<<(std::ostream &output, const Storage *storage);
			
			int getDatabaseIndex(std::string databaseName);
			int getTableIndex(std::string databaseName, std::string tableName);
			int getRecordIndex(std::string databaseName, std::string tableName, int recordID);

			std::vector<std::string> getDatabaseNames() const;
			std::vector<std::string> getTableNames(std::string databaseName) const;

			std::string to_string() const;

			std::string to_gui_string() const;
	};
}
#endif