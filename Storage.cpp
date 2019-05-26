#include "pch.h"
#include "Storage.h"

/*!
 * Storage.h is the main header for storage part 
 */
namespace dbms {
	std::ostream & operator<<(std::ostream & output, const Storage  *storage)
	{
		output << storage->to_string();
		return output;
	}

	std::string Storage::to_string() const
	{
		try {
			std::string storageInfo = "";
			for (size_t i = 0; i < this->databases.size(); i++) {	// loop through all databases
				storageInfo.append("\t<Database>: [" + this->databases[i]->databaseName + "]\n");
				for (size_t k = 0; k < this->databases[i]->tables.size(); k++) {	// loop through tables in current database
					storageInfo.append("\t\t<Table>: [" + this->databases[i]->tables[k]->tableName + "]");
					storageInfo.append(" {");
					for (size_t col = 0; col < this->databases[i]->tables[k]->columnNames.size(); col++) {
						storageInfo.append(this->databases[i]->tables[k]->columnNames[col]);
						if (col != this->databases[i]->tables[k]->columnNames.size() - 1) {
							storageInfo.append(", ");
						}
					}
					storageInfo.append("}\n");
					for (size_t j = 0; j < this->databases[i]->tables[k]->records.size(); j++) {	// loop through records in current table
						storageInfo.append("\t\t\t<RecordID>: [" + std::to_string(this->databases[i]->tables[k]->records[j]->recordID) + "]\n");
						// loop through the table columns and corresponding values
						for (size_t h = 0; h < this->databases[i]->tables[k]->columnNames.size(); h++) {
							storageInfo.append("\t\t\t\t<data>(" + this->databases[i]->tables[k]->columnNames[h] + "): [" + this->databases[i]->tables[k]->records[j]->dataFields[h] + ']' + '\n');
						}
					}
				}
			}
			return storageInfo;
		}
		catch (const std::exception e) {
			
		}
		return "";
	}

	std::string Storage::to_gui_string() const
	{
		std::string storageInfo = "";
		for (Database *database : this->databases) {	// loop through all databases
			storageInfo.append(database->databaseName + "                     " + "database\n");
			for (Table* table : database->tables) {
				storageInfo.append("    ");
				storageInfo.append(table->tableName + "                     " + "table\n");
			}
		}
		return storageInfo;
	}

	Storage::~Storage()
	{
		// free all memory used by storage (all databases, tables, and records)
		for (size_t i = 0; i < this->databases.size(); i++) {
			for (size_t k = 0; k < this->databases[i]->tables.size(); k++) {
				for (size_t j = 0; j < this->databases[i]->tables[k]->records.size(); j++) {
					delete(this->databases[i]->tables[k]->records[j]);
					this->databases[i]->tables[k]->records[j] = nullptr;
				}
				delete(this->databases[i]->tables[k]);
				this->databases[i]->tables[k] = nullptr;
			}
			delete(this->databases[i]);
			this->databases[i] = nullptr;
		}
	}

	int Storage::getDatabaseIndex(std::string databaseName)
	{
		int index = 0;
		while (this->databases[index]->databaseName != databaseName) {
			index++;
			if (index == this->databases.size())
				return -1;
		}
		return index;
	}
	
	int Storage::getTableIndex(std::string databaseName, std::string tableName)
	{
		int databaseIndex = this->getDatabaseIndex(databaseName);
		
		int tableIndex = 0;
		while (this->databases[databaseIndex]->tables[tableIndex]->tableName != tableName) {
			tableIndex++;
			if (tableIndex == this->databases[databaseIndex]->tables.size())
				return -1;
		}
		return tableIndex;
	}

	int Storage::getRecordIndex(std::string databaseName, std::string tableName, int recordID)
	{
		int databaseIndex = this->getDatabaseIndex(databaseName);
		int tableIndex = this->getTableIndex(databaseName, tableName);
		int recordIndex = 0;
		while (this->databases[databaseIndex]->tables[tableIndex]->records[recordIndex]->recordID != recordID) {
			recordIndex++;
			if (recordIndex == this->databases[databaseIndex]->tables[tableIndex]->records.size())
				return -1;
		}
		return recordIndex;
	}

	std::vector<std::string> Storage::getDatabaseNames() const
	{
		std::vector<std::string> databaseNames;
		for (size_t i = 0; i < this->databases.size(); i++) {
			databaseNames.push_back(this->databases[i]->databaseName);
		}
		return databaseNames;
	}

	std::vector<std::string> Storage::getTableNames(std::string databaseName) const
	{
		std::vector<std::string> tableNames;
		for (size_t i = 0; i < this->databases.size(); i++) {
			if (this->databases[i]->databaseName == databaseName) {
				for (size_t k = 0; k < this->databases[i]->tables.size(); k++) {
					tableNames.push_back(this->databases[i]->tables[k]->tableName);
				}
			}
		}
		return tableNames;
	}
}