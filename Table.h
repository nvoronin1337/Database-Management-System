#pragma once
#ifndef TABLE
#define TABLE


#include "Record.h"
#include <string>
#include <vector>
#include <iterator>

namespace dbms {
	class Table {
		public:
			std::string tableName;
			int numOfRecords = 0;
			std::vector<std::string> columnNames;
			std::vector<Record*> records;

			Table(std::string tableName);
			~Table() = default;

			friend std::ostream &operator<<(std::ostream &output, const Table *table);
	};
}


#endif

