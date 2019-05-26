#include "pch.h"
#include "StorageManager.h"


namespace dbms {
	Storage * StorageManager::getStorage() const
	{
		return this->storage;
	}
	// CONSTRUCTOR
	StorageManager::StorageManager(Storage* storage)
	{
		this->storage = storage;
	}

	// GETTERS FOR STORAGE
	Database* StorageManager::getDatabase(std::string databaseName) const
	{
		int databaseIndex = this->storage->getDatabaseIndex(databaseName);
		return this->storage->databases[databaseIndex];
	}

	Table* StorageManager::getTable(std::string databaseName, std::string tableName) const
	{
		int databaseIndex = this->storage->getDatabaseIndex(databaseName);
		int tableIndex = this->storage->getTableIndex(databaseName, tableName);
		return this->storage->databases[databaseIndex]->tables[tableIndex];
	}

	Record* StorageManager::getRecord(std::string databaseName, std::string tableName, int recordID) const
	{
		int databaseIndex = this->storage->getDatabaseIndex(databaseName);
		int tableIndex = this->storage->getTableIndex(databaseName, tableName);
		int recordIndex = this->storage->getRecordIndex(databaseName, tableName, recordID);
		return this->storage->databases[databaseIndex]->tables[tableIndex]->records[recordIndex];
	}

	// ADD / DELETE DATABASE
	void StorageManager::addDatabase(std::string databaseName)
	{
		Database* database = new Database(databaseName);
		this->storage->databases.push_back(database);
	}

	void StorageManager::deleteDatabase(std::string databaseName)
	{
		int databaseIndex = this->storage->getDatabaseIndex(databaseName);
		this->storage->databases.erase(storage->databases.begin() + databaseIndex);
		//delete(this->storage->databases[databaseIndex]);
		//this->storage->databases[databaseIndex] = nullptr;
	}

	// ADD / DELETE TABLE
	void StorageManager::addTableToDatabase(std::string databaseName, std::string tableName, std::vector <std::string> tableColumns)
	{
		int databaseIndex = this->storage->getDatabaseIndex(databaseName);
		Table* table = new Table(tableName);
		if (tableColumns.size() > 0) {
			table->columnNames = tableColumns;
		}
		this->storage->databases[databaseIndex]->tables.push_back(table);	
	}

	void StorageManager::deleteTableFromDatabase(std::string databaseName, std::string tableName)
	{
		int databaseIndex = this->storage->getDatabaseIndex(databaseName);
		int tableIndex = this->storage->getTableIndex(databaseName, tableName);
		this->storage->databases[databaseIndex]->tables.erase(storage->databases[databaseIndex]->tables.begin() + tableIndex);
		//delete(this->storage->databases[databaseIndex]->tables[tableIndex]);
		//this->storage->databases[databaseIndex]->tables[tableIndex] = nullptr;
	}

	// ADD / DELETE RECORD
	void StorageManager::addRecordToTableInDatabase(std::string databaseName, std::string tableName, std::vector<std::string> recordValues)
	{
		int databaseIndex = this->storage->getDatabaseIndex(databaseName);
		int tableIndex = this->storage->getTableIndex(databaseName, tableName);
		Record* record = new Record();
		
		this->storage->databases[databaseIndex]->tables[tableIndex]->numOfRecords++;
		int recordID = this->storage->databases[databaseIndex]->tables[tableIndex]->numOfRecords;
		record->recordID = recordID;
		record->dataFields = recordValues;
		this->storage->databases[databaseIndex]->tables[tableIndex]->records.push_back(record);

	}

	void StorageManager::deleteRecordFromTableInDatabase(std::string databaseName, std::string tableName, int recordID)
	{
		int databaseIndex = this->storage->getDatabaseIndex(databaseName);
		int tableIndex = this->storage->getTableIndex(databaseName, tableName);
		int recordIndex = this->storage->getRecordIndex(databaseName, tableName, recordID);

		this->storage->databases[databaseIndex]->tables[tableIndex]->records.erase(storage->databases[databaseIndex]->tables[tableIndex]->records.begin() + recordIndex);
		this->storage->databases[databaseIndex]->tables[tableIndex]->numOfRecords--;

		//refresh ID values for every record in this table
		for (int i = 0; i < this->storage->databases[databaseIndex]->tables[tableIndex]->records.size(); i++) {
			this->storage->databases[databaseIndex]->tables[tableIndex]->records[i]->recordID = (i + 1);
		}
	}
	
	void StorageManager::updateRecordInTableInDatabase(std::string databaseName, std::string tableName, int recordID, std::vector<std::string> newValues)
	{
		int databaseIndex = this->storage->getDatabaseIndex(databaseName);
		int tableIndex = this->storage->getTableIndex(databaseName, tableName);
		int recordIndex = this->storage->getRecordIndex(databaseName, tableName, recordID);
		
		int numOfNewValues = newValues.size();
		int numOfCurrentValues = storage->databases[databaseIndex]->tables[tableIndex]->records[recordIndex]->dataFields.size();

		if (numOfNewValues == numOfCurrentValues) {
			storage->databases[databaseIndex]->tables[tableIndex]->records[recordIndex]->dataFields = newValues;
		}
	}
	bool StorageManager::hasDatabase(std::string databaseName) const
	{
		for (int i = 0; i < this->getStorage()->databases.size(); i++) {
			if (this->getStorage()->databases[i]->databaseName == databaseName) {
				return true;
			}
		}
		return false;
	}
	bool StorageManager::hasTable(std::string databaseName, std::string tableName) const
	{
		for (int i = 0; i < this->getStorage()->databases.size(); i++) {
			if (this->getStorage()->databases[i]->databaseName == databaseName) {
				for (int k = 0; k < this->getStorage()->databases[i]->tables.size(); k++) {
					if (this->getStorage()->databases[i]->tables[k]->tableName == tableName) {
						return true;
					}
				}
			}
		}
		return false;
	}
	bool StorageManager::deleteDatabase(int db_index)
	{
		this->storage->databases.erase(this->storage->databases.begin() + db_index, this->storage->databases.begin() + db_index + 1);
		return true;
	}
}