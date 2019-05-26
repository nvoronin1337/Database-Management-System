#pragma once
#ifndef CONSOLE_UI
#define CONSOLE_UI

#include "StorageManager.h"
#include "Registrator.h"
#include "Login.h"
#include "PasswordChecker.h"
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

#ifdef _WIN32
#include <conio.h>
#endif	//WIN32

namespace dbms {
	enum class LogInOptions {
		Register = 1,
		LogIn
	};

	enum class Options {
		ShowAllData = 1,
		AddDatabase,
		DeleteDatabase,
		AddTable,
		DeleteTable,
		AddRow,
		DeleteRow,
		EditRecord,
		UploadData,
		Exit = 0
	};

	struct User {
		std::string userName;
		Storage *storage = nullptr;
	};

	class ConsoleUI {
	private:
		std::unique_ptr<User> user;
		StorageManager* storageManager;
		std::unique_ptr<Parser> parser;
	public:
		ConsoleUI() = default;
		ConsoleUI(StorageManager* storageManager);
		~ConsoleUI();

		// Start UI (calls other methods if necessary)
		void getReady();
		void init();

		// Show menu
		int showLogInOptions() const;
		int showOptions() const;

		void addDatabase();
		void deleteDatabase();

		void addTableToDatabase();
		void deleteTableFromDatabase();

		void addRecord();
		void deleteRecord();

		void editRecord();

		void uploadFile() const;

		void printDatabases() const;
		void printTablesFromDatabase(std::string databaseName) const;
		void printRecordsFromTable(std::string databaseName, std::string tableName) const;
	};
}
#endif //!ConsoleUI