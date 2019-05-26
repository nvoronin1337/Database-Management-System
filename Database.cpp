#include "pch.h"
#include "Database.h"

namespace dbms {

	Database::Database(std::string databaseName)
	{
		this->databaseName = databaseName;
	}
	std::ostream & operator<<(std::ostream & output, const Database* database)
	{
		try {
			std::string databaseInfo = "";
			for (size_t k = 0; k < database->tables.size(); k++) {	// loop through tables in current database
				databaseInfo.append("\t\t<Table>: [" + database->tables[k]->tableName + "]");
				databaseInfo.append(" {");
				for (size_t col = 0; col < database->tables[k]->columnNames.size(); col++) {
					databaseInfo.append(database->tables[k]->columnNames[col]);
					if (col != database->tables[k]->columnNames.size() - 1) {
						databaseInfo.append(", ");
					}
				}
				databaseInfo.append("}\n");
				for (size_t j = 0; j < database->tables[k]->records.size(); j++) {	// loop through records in current table
					databaseInfo.append("\t\t\t<RecordID>: [" + std::to_string(database->tables[k]->records[j]->recordID) + "]\n");
					// loop through the table columns and corresponding values
					for (size_t h = 0; h < database->tables[k]->columnNames.size(); h++) {
						databaseInfo.append("\t\t\t\t<data>(" + database->tables[k]->columnNames[h] + "): [" + database->tables[k]->records[j]->dataFields[h] + "]\n");
					}
				}
			}
			output << databaseInfo;
			return output;
		}
		catch (const std::exception e) {

		}
	}
}
