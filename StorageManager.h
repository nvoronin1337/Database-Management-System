#pragma once
#ifndef STORAGE_MANAGER
#define STORAGE_MANAGER
#include <string>


#include "Storage.h"
#include "Database.h"
#include "Table.h"
#include "Record.h"


namespace dbms {
	class StorageManager {
		//! dbms::StorageManager class.

		/**	\class StorageManager
		 * dbms::StorageManager class is used to manipulate data in the storage.
		 * Private attributes: pointer to the Storage onject
		 * All the methods of this class are public.
		 * 
		 * Class contains methods to:
		 *	1) Add and delete specified database from storage.
		 *	2) Add and delete specified table from the specified database.
		 *	3) Add and delete specified record from the specified table from the specified database.
		 *	4) To be added: Edit specified record.
		 *	5) Check if the storage contains specified Database.
		 *	6) Check if the storage contains specified Table.
		 *	7) Accessors for the specified Database, Table, and Record.
		*/
	private:
		Storage* storage;
	public:
		Storage* getStorage() const;

		StorageManager(Storage* storage);
		~StorageManager() = default;

		Database* getDatabase(std::string databaseName) const;
		Table* getTable(std::string databaseName, std::string tableName) const;
		Record* getRecord(std::string databaseName, std::string tableName, int recordID) const;

		
		void addDatabase(std::string databaseName);
		void deleteDatabase(std::string databaseName);

		void addTableToDatabase(std::string databaseName, std::string tableName, std::vector <std::string> tableColumns);
		void deleteTableFromDatabase(std::string databaseName, std::string tableName);

		void addRecordToTableInDatabase(std::string databaseName, std::string tableName, std::vector<std::string> recordValues);
		void deleteRecordFromTableInDatabase(std::string databaseName, std::string tableName, int recordID);

		void updateRecordInTableInDatabase(std::string databaseName, std::string tableName, int recordID, std::vector<std::string> recordValues);

		bool hasDatabase(std::string databaseName) const;
		bool hasTable(std::string databaseName, std::string tableName) const;


		// Added for GUI
		bool deleteDatabase(int db_index);
		bool deleteTable(int db_index, int tableIndex);
		bool deleteRecord(int db_index, int table_index, int record_index);
	};
}

#endif