#include "pch.h"

#include "ConsoleUI.h"


namespace dbms {
	ConsoleUI::ConsoleUI(StorageManager * storageManager) {
		this->storageManager = storageManager;
	}

	ConsoleUI::~ConsoleUI()
	{
		auto helper = std::make_unique<FileIOHelper>();
		helper->saveStorageText(this->storageManager->getStorage(), this->user->userName + ".txt");
		helper->saveStorageBinary(this->storageManager->getStorage(), this->user->userName + ".bin");
	}

	void ConsoleUI::getReady()
	{
		int choice = showLogInOptions();
		std::string userName = "";
		std::string password = "";
		bool stop = false;
		while (!stop) {
			switch (choice) {
				case static_cast<int>(LogInOptions::Register) : {
					
					std::cout << "\t*Please enter new user name: " << std::endl;
					std::cout << "\t*";
					std::cin >> userName;

					bool is_good_pass = false;
					auto password_checker = std::make_unique<PasswordChecker>();
					
					//working version 
					std::cout << "\t*Please enter new password: " << std::endl;
					std::cout << "\t*";
					std::cin >> password;

					/* Not working version.
					while (!is_good_pass) {
						std::cout << "\t*Please enter new password: " << std::endl;
						std::cout << "\t*";
						std::cin >> password;

						std::pair<bool, std::string> pass_check = password_checker->checkPassword(password);
						
						if (pass_check.first == true)
							is_good_pass = true;
						else {
							std::cout << "\t*" + pass_check.second << std::endl;
						}
					}
					*/

					auto logger = std::make_unique<Registrator>(userName, password);

					this->user = std::make_unique <User>();
					this->user->userName = userName;
					auto* storage = new Storage();
					this->user->storage = storage;
					this->storageManager = new StorageManager(storage);

					auto fileHelper = std::make_unique<FileIOHelper>();

					try {
						auto* parser = new Parser(this->storageManager, fileHelper->loadStorageText(user->userName + ".txt"));
						this->parser = std::make_unique<Parser>(*parser);
						this->parser->parseConfig();
					}
					catch (const std::runtime_error ex) {
						//std::cerr << ex.what() << std::endl;

						auto* parser = new Parser();
						this->parser = std::make_unique<Parser>(*parser);

						this->parser->setStorageManager(this->storageManager);
					}

					init();
					stop = true;
					break;
				}
				case static_cast<int>(LogInOptions::LogIn) : {
					
					bool isUser = false;
					while (!isUser) {
						userName.clear();
						password.clear();
						std::cout << "\t*Please enter your user name: " << std::endl;
						std::cout << "\t*=>";
						std::cin >> userName;
						std::cout << "\t*Please enter your password: " << std::endl;
						std::cout << "\t*=>";

						// Hiding password with '*'.WINDOWS ONLY
						#ifdef _WIN32
						char ch = _getch();
						while (ch != 13) {
							password.push_back(ch);
							std::cout << '*';
							ch = _getch();
						}
						#else
						std::cin >> password;	// for not windows os 
						#endif // end WIN32 

						try {
							auto logger = std::make_unique<Login>(userName, password);
							isUser = true;
							std::cout << "\n\t*Welcome " << userName << "!\n" << std::endl;
						}
						catch (const std::invalid_argument &e) {
							std::cout << e.what() << std::endl;
						}
					}
					auto* storage = new Storage();
					this->storageManager = new StorageManager(storage);

					this->user = std::make_unique<User>();
					this->user->userName = userName;
					this->user->storage = storage;

					auto fileHelper = std::make_unique<FileIOHelper>();
					auto* parser = new Parser(this->storageManager, fileHelper->loadStorageText(user->userName + ".txt"));
					
					this->parser = std::make_unique<Parser>(*parser);
					this->parser->parseConfig();
				
					init();
					stop = true;
					break;
				}
				default:
					std::cout << "\t*Invalid key." << std::endl;
					choice = showLogInOptions();
					break;
			}
		}
		return;
	}

	void ConsoleUI::init()
	{

		// read storage from file
		if (this->storageManager->getStorage()->databases.size() < 0) {
			std::cout << "\t*Your storage is currently empty." << std::endl;
		}
		bool stop = false;
		while (!stop) {
			int choice = showOptions();
			
			std::cout << "\t*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*" << std::endl;
			switch (choice) {
				case static_cast<int>(Options::ShowAllData) :	// case 1	SHOW ALL STORAGE DATA
				{
					std::cout << storageManager->getStorage();
					break;
				}
				case static_cast<int>(Options::AddDatabase) :	// case 2	ADD DATABASE
				{
					addDatabase();
					break;
				}
				case static_cast<int>(Options::DeleteDatabase) :	// case 3 DELETE DATABASE
				{
					deleteDatabase();
					break;
				}
				case static_cast<int>(Options::AddTable) : {	// case 4 ADD TABLE
					addTableToDatabase();
					break;
				}
				case static_cast<int>(Options::DeleteTable) : {	// case 5 DELETE TABLE
					deleteTableFromDatabase();
					break;
				}
				case static_cast<int>(Options::AddRow) :	// case 6 ADD RECORD
				{
					addRecord();
					break;
				}
				case static_cast<int>(Options::DeleteRow) :	// case 7 DELETE RECORD
				{
					deleteRecord();
					break;
				}
				case static_cast<int>(Options::EditRecord) :	// case 8 EDIT RECORD
				{
					editRecord();
					break;
				}
				case static_cast<int>(Options::UploadData) :	// case 9 UploadData
				{
					uploadFile();
					break;
				}
				// add other cases
				case static_cast<int>(Options::Exit) :	// case 0 EXIT
				{
					stop = true;
					std::cout << "\t*Your storage currenty: " << std::endl;
					std::cout << "\t-----------------------------------------------" << std::endl;
					std::cout << this->storageManager->getStorage();
					std::cout << "\t-----------------------------------------------" << std::endl;
					std::cout << "\t*Thank you for testing my application." << std::endl;
					break;
				}
				default:
				{
					std::cout << "\t*Invalid key.\n" << std::endl;
					choice = 0;
					break;
				}
			}
			std::cout << "\t*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*" << std::endl;
			std::cout << std::endl;
		}
		return;
	}

	int ConsoleUI::showLogInOptions() const
	{
		int choice = 0;
		std::cout << "\n";
		std::cout << "\t****************************************" << std::endl;
		std::cout << "\t*              Log In                  *" << std::endl;
		std::cout << "\t* Please select one of the options:    *" << std::endl;
		std::cout << "\t* 1. Register.                         *" << std::endl;
		std::cout << "\t* 2. Log in.                           *" << std::endl;
		std::cout << "\t****************************************" << std::endl;
		std::cout << "\tPlease select option: ";

		try {
			std::cin >> choice;
		}
		catch (std::exception& ex) {
			std::cerr << "Error! " << ex.what() << std::endl;
			choice = 0;
		}
		return choice;
	}

	int ConsoleUI::showOptions() const {
		int choice = 0;

		std::cout << "\t****************************************" << std::endl;
		std::cout << "\t*              MENU                    *" << std::endl;
		std::cout << "\t* Please select one of the options:    *" << std::endl;
		std::cout << "\t* 1. Show storage contents.            *" << std::endl;
		std::cout << "\t* 2. Add database to the storage.      *" << std::endl;
		std::cout << "\t* 3. Delete database from the storage. *" << std::endl;
		std::cout << "\t* 4. Add table to the database.        *" << std::endl;
		std::cout << "\t* 5. Delete table from the database.   *" << std::endl;
		std::cout << "\t* 6. Add data into table.              *" << std::endl;
		std::cout << "\t* 7. Delete data from table.           *" << std::endl;
		std::cout << "\t* 8. Edit data.                        *" << std::endl;
		std::cout << "\t* 9. Upload Files.                     *" << std::endl;
		std::cout << "\t* 0. Exit.                             *" << std::endl;
		std::cout << "\t****************************************" << std::endl;
		std::cout << "\tPlease select option: ";
		try {
			std::cin >> choice;
		}
		catch (std::exception& ex) {
			std::cerr << "Error! " << ex.what() << std::endl;
			choice = 0;
		}
		std::cout << std::endl;

		return choice;
	}

	void ConsoleUI::addDatabase()
	{
		std::string input = "";
		std::string newDatabaseName = "";

		std::cout << "\t\t\t ADD DATABASE" << std::endl;

		while (true) {
			printDatabases();
			std::cout << std::endl;
			std::cout << "\t*Please enter name of the new Database: " << std::endl;
			std::cout << "\t*Please enter STOP if you dont want to add any databases." << std::endl;
			std::cout << "\t*";
			std::cin >> input;
			input = input.substr(0, input.find(" "));
			std::cout << std::endl;

			if (input == "STOP" || input == "stop") {
				std::cout << "\t*Returning back to menu..." << std::endl;
				return;
			}
			else {
				newDatabaseName = input;
				this->storageManager->addDatabase(newDatabaseName);
				std::cout << "\t*Do you want to add another database? (y/n): " << std::endl;
				std::cout << "\t*";
				std::cin >> input;
				if (input != "y" && input != "yes" && input != "Y" && input != "YES") {
					std::cout << "\t*Returning back to menu..." << std::endl;
					return;
				}
			}
		}
	}

	void ConsoleUI::deleteDatabase()
	{
		
		std::string input = "";
		std::string databaseToDelete = "";
		std::vector<std::string> databaseNames;
		
		while (true) {
			if (this->storageManager->getStorage()->databases.size() != 0) {
				databaseNames = this->storageManager->getStorage()->getDatabaseNames();
				printDatabases();
				std::cout << "\t*Please choose the databases you want to delete: " << std::endl;
				std::cout << "\t*Please enter STOP if you dont want to delete any databases." << std::endl;
				std::cout << "\t*";
				std::cin >> input;
				std::cout << std::endl;

				if (input == "STOP" || input == "stop") {
					std::cout << "\t*Returning back to menu..." << std::endl;
					return;
				}
				else {
					databaseToDelete = input;
				}

				if (this->storageManager->hasDatabase(databaseToDelete)) {
					this->storageManager->deleteDatabase(databaseToDelete);

					std::cout << "\t*Do you want to delete another database? (y/n): " << std::endl;
					std::cout << "\t*";
					std::cin >> input;
					if (input != "y" && input != "yes" && input != "Y" && input != "YES") {
						std::cout << "\t*Returning back to menu..." << std::endl;
						return;
					}
				}
				else {
					std::cout << "\t*You have entered incorrect database name!" << std::endl;
				}
			}
			else {
				std::cout << "\t*There are no databases in your storage." << std::endl;
				return;
			}
		}
	}

	void ConsoleUI::addTableToDatabase()
	{
		std::string input = "";
		std::string databaseName = "";
		std::string tableToAdd = "";
		std::vector<std::string> databaseNames;
		std::vector<std::string> tableNames;
		std::vector<std::string> columnNames;
		bool databaseIsPresent = false;
		
		while (!databaseIsPresent) {
			printDatabases();
			std::cout << "\t*Please choose the database to add table in." << std::endl;
			std::cout << "\t*Please enter STOP if you dont want to delete any databases." << std::endl;
			std::cout << "\t*";
			std::cin >> input;
			std::cout << std::endl;

			if (input == "STOP" || input == "stop") {
				std::cout << "\t* Returning back to menu..." << std::endl;
				return;
			}
			else {
				databaseName = input;
			}

			if (this->storageManager->hasDatabase(databaseName)) {
				databaseIsPresent = true;
				
				while (true) {
					printTablesFromDatabase(databaseName);
					std::cout << "\t* Please enter name of a new table" << '.' << std::endl;
					std::cout << "\t* Please enter STOP if you dont want to add a table." << std::endl;
					std::cout << "\t*";
					std::cin >> input;
					input = input.substr(0, input.find(" "));
					std::cout << std::endl;

					if (input == "STOP" || input == "stop") {
						std::cout << "\t* Returning back to menu..." << std::endl;
						return;
					}
					else {
						tableToAdd = input;
						input.clear();

						std::cout << "\t*Please enter column names for the table." << std::endl;
						std::cout << "\t*Please enter STOP to finish adding columns." << std::endl;
						int count = 1;
						bool stop = false;
						while (!stop) {
							std::cout << "\t*" << "Column " << count << ": ";
							std::cin >> input;
							input = input.substr(0, input.find(" "));
							if (input != "stop" && input != "STOP") {
								columnNames.push_back(input);
								count++;
							}
							else {
								stop = true;
							}
							input.clear();
						}
						if (columnNames.size() == 0) {
							columnNames.push_back("empty");
						}
						this->storageManager->addTableToDatabase(databaseName, tableToAdd, columnNames);
						std::cout << "\t*Do you want to add another table? (y/n): " << std::endl;
						std::cout << "\t*";
						std::cin >> input;
						if (input != "y" && input != "yes" && input != "Y" && input != "YES") {
							std::cout << "\t*Returning back to menu..." << std::endl;
							return;
						}
						else {
							columnNames.clear();
						}
					}
				}
			}
			else {
				std::cout << "\t*You have entered incorrect database name!" << std::endl;
			}
		}
	}

	void ConsoleUI::deleteTableFromDatabase()
	{
		std::string input = "";
		std::string databaseName = "";
		std::string tableToDelete = "";
		std::vector<std::string> databaseNames;
		std::vector<std::string> tableNames;
		std::vector<std::string> columnNames;
		bool databaseIsPresent = false;

		while (!databaseIsPresent) {
			printDatabases();
			std::cout << "\t*Please choose the database to delete table from." << std::endl;
			std::cout << "\t*Please enter STOP if you dont want to delete any tables." << std::endl;
			std::cout << "\t*";
			std::cin >> input;
			std::cout << std::endl;

			if (input == "STOP" || input == "stop") {
				std::cout << "\t* Returning back to menu..." << std::endl;
				return;
			}
			else {
				databaseName = input;
			}

			if (this->storageManager->hasDatabase(databaseName)) {
				databaseIsPresent = true;

				while (true) {
					if (this->storageManager->getDatabase(databaseName)->tables.size() != 0) {
						printTablesFromDatabase(databaseName);
						std::cout << "\t* Please enter name of a table to be deleted" << '.' << std::endl;
						std::cout << "\t* Please enter STOP if you dont want to delete a table." << std::endl;
						std::cout << "\t*";
						std::cin >> input;
						std::cout << std::endl;

						if (input == "STOP" || input == "stop") {
							std::cout << "\t* Returning back to menu..." << std::endl;
							return;
						}
						else {
							tableToDelete = input;
							if (this->storageManager->hasTable(databaseName, tableToDelete)) {
								this->storageManager->deleteTableFromDatabase(databaseName, tableToDelete);
							}
							else {
								std::cout << "No table found..." << std::endl;
							}
							std::cout << "\t*Do you want to delete another table? (y/n): " << std::endl;
							std::cout << "\t*";
							std::cin >> input;
							if (input != "y" && input != "yes" && input != "Y" && input != "YES") {
								std::cout << "\t*Returning back to menu..." << std::endl;
								return;
							}
						}
					}
					else {
						std::cout << "\t*There are no tables in this database." << std::endl;
						return;
					}
				}
			}
		}

	}

	void ConsoleUI::addRecord()
	{
		std::string input = "";
		std::string databaseName = "";
		std::string tableName = "";
		std::vector<std::string> databaseNames;
		std::vector<std::string> tableNames;
		std::vector<std::string> columnNames;
		std::vector<std::string> values;
		bool databaseIsPresent = false;
		bool tableIsPresent = false;
		bool tableHasColumns = false;

		while (!databaseIsPresent) {
			printDatabases();
			std::cout << "\t*Please choose the database to work with." << std::endl;
			std::cout << "\t*Please enter STOP to go back to menu." << std::endl;
			std::cout << "\t*";
			std::cin >> input;
			std::cout << std::endl;

			if (input == "STOP" || input == "stop") {
				std::cout << "\t* Returning back to menu..." << std::endl;
				return;
			}
			else {
				databaseName = input;
			}

			if (this->storageManager->hasDatabase(databaseName)) {
				databaseIsPresent = true;

				printTablesFromDatabase(databaseName);
				std::cout << "\t*Please enter name of a table to work with." << std::endl;
				std::cout << "\t*Please enter STOP to go back to menu." << std::endl;
				std::cout << "\t*";
				std::cin >> input;
				std::cout << std::endl;

				if (input == "STOP" || input == "stop") {
					std::cout << "\t* Returning back to menu..." << std::endl;
					return;
				}
				else {
					tableName = input;
					input = "";

					// if there are no columns -> add columns
					while (this->storageManager->getTable(databaseName, tableName)->columnNames.size() == 0) {
						std::cout << "\t*This table doesn't have any columns yet." << std::endl;
						std::cout << "\t*Please enter column names for the table." << std::endl;
						std::cout << "\t*Please enter STOP to finish adding columns." << std::endl;
						int count = 1;
						while (true) {
							std::cout << "\t*" << "Column " << count << ": ";
							std::cin >> input;
							if (input == "STOP" || input == "stop") {
								std::cout << "\t*Returning back to menu..." << std::endl;
								return;
							}
							columnNames.push_back(input);
							count++;
						}
						if (columnNames.size() == 0) {
							columnNames.push_back("empty");
						}
						this->storageManager->getTable(databaseName, tableName)->columnNames = columnNames;
						columnNames.clear();
					}
					
					while (true) {
						std::cout << "\t*Please enter values for every column." << std::endl;
						for (size_t j = 0; j < this->storageManager->getTable(databaseName, tableName)->columnNames.size(); j++) {
							std::cout << "\t*" << this->storageManager->getTable(databaseName, tableName)->columnNames[j] << ": ";
							std::cin >> input;
							values.push_back(input);
						}
						this->storageManager->addRecordToTableInDatabase(databaseName, tableName, values);
						std::cout << "\t*Do you want to add another record? (y/n): " << std::endl;
						std::cout << "\t*";
						std::cin >> input;
						if (input != "y" && input != "yes" && input != "Y" && input != "YES") {
							std::cout << "\t*Returning back to menu..." << std::endl;
							return;
						}
						else {
							values.clear();
						}
					}
					return;
				}
			}
			else {
				std::cout << "\t*You have entered incorrect database name!" << std::endl;
			}
		}
	}

	void ConsoleUI::deleteRecord()
	{
		std::string input = "";
		std::string databaseName = "";
		std::string tableName = "";
		std::vector<std::string> databaseNames;
		std::vector<std::string> tableNames;
		bool databaseIsPresent = false;
		bool tableIsPresent = false;
		bool tableHasColumns = false;

		while (!databaseIsPresent) {
			printDatabases();
			std::cout << "\t*Please choose the database to work with." << std::endl;
			std::cout << "\t*Please enter STOP to go back to menu." << std::endl;
			std::cout << "\t*";
			std::cin >> input;
			std::cout << std::endl;

			if (input == "STOP" || input == "stop") {
				std::cout << "\t*Returning back to menu..." << std::endl;
				return;
			}
			else {
				databaseName = input;
			}

			if (this->storageManager->hasDatabase(databaseName)) {
				databaseIsPresent = true;

				input = "";
				while (!this->storageManager->hasTable(databaseName, input)) {
					printTablesFromDatabase(databaseName);
					std::cout << "\t*Please enter name of a table to work with." << std::endl;
					std::cout << "\t*Please enter STOP to go back to menu." << std::endl;
					std::cout << "\t*";
					std::cin >> input;
					std::cout << std::endl;
					if (input == "STOP" || input == "stop") {
						std::cout << "\t*Returning back to menu..." << std::endl;
						return;
					}
					else {
						tableName = input;
						if (!this->storageManager->hasTable(databaseName, tableName)) {
							std::cout << "\t*You have entered incorrect table name!" << std::endl;
						}
					}
				}
				if (this->storageManager->getTable(databaseName, tableName)->records.size() == 0) {
					std::cout << "\t*There are no records in this table." << std::endl;
					return;
				}
				input = "";
				int selection = -1;
				while (true) {
					while (selection < 1) {
						printRecordsFromTable(databaseName, tableName);
						std::cout << "\t*Please enter id of a record to be deleted." << std::endl;
						std::cout << "\t*Please enter STOP to go back to menu." << std::endl;
						std::cout << "\t*";
						std::cin >> input;
						if (input == "stop" || input == "STOP")
							return;
						try {
							selection = std::stoi(input);
							this->storageManager->deleteRecordFromTableInDatabase(databaseName, tableName, selection);
							if (this->storageManager->getTable(databaseName, tableName)->records.size() != 0) {
								std::cout << "\t*Do you want to delete another record? (y/n): " << std::endl;
								std::cout << "\t*";
								std::cin >> input;
								if (input != "y" && input != "yes" && input != "Y" && input != "YES") {
									std::cout << "\t*Returning back to menu..." << std::endl;
									return;
								}
								else {
									selection = -1;
								}
							}
							else {
								std::cout << "\t*There are no more records in this table." << std::endl;
								return;
							}
						}
						catch (std::exception ex) {
							std::cout << "\t*invalid input" << std::endl;
						}
					}
				}
			}
			else {
				std::cout << "\t*You have entered incorrect database name!" << std::endl;
			}
		}
	}

	void ConsoleUI::editRecord()
	{
		std::string input = "";
		std::string databaseName = "";
		std::string tableName = "";
		std::vector<std::string> databaseNames;
		std::vector<std::string> tableNames;
		std::vector<std::string> columnNames;
		std::vector<std::string> values;
		bool databaseIsPresent = false;
		bool tableIsPresent = false;
		bool tableHasColumns = false;

		while (!databaseIsPresent) {
			printDatabases();
			std::cout << "\t*Please choose the database to work with." << std::endl;
			std::cout << "\t*Please enter STOP to go back to menu." << std::endl;
			std::cout << "\t*";
			std::cin >> input;
			std::cout << std::endl;

			if (input == "STOP" || input == "stop") {
				std::cout << "\t* Returning back to menu..." << std::endl;
				return;
			}
			else {
				databaseName = input;
			}

			if (this->storageManager->hasDatabase(databaseName)) {
				databaseIsPresent = true;

				while (!tableIsPresent) {
					printTablesFromDatabase(databaseName);
					std::cout << "\t*Please enter name of a table to work with." << std::endl;
					std::cout << "\t*Please enter STOP to go back to menu." << std::endl;
					std::cout << "\t*";
					std::cin >> input;
					std::cout << std::endl;

					if (input == "STOP" || input == "stop") {
						std::cout << "\t* Returning back to menu..." << std::endl;
						return;
					}
					else if(this->storageManager->hasTable(databaseName, input)){
						tableIsPresent = true;
						tableName = input;
						input = "";
					}
				}
				
				if (this->storageManager->getTable(databaseName, tableName)->records.size() == 0) {
					std::cout << "\t* There are no records in this table." << std::endl;
					std::cout << "\t* Returning back to menu..." << std::endl;
					return;
				}
				else {

					std::cout << this->storageManager->getTable(databaseName, tableName);
					int size = this->storageManager->getTable(databaseName, tableName)->records.size();
					int id = -1;
					bool found = false;
					while (!found) {
						std::cout << "\t* Please enter a record id of a record that you want to change:\n";
						std::cout << "\t* ";
						std::cin >> id;
						id--;
						if (id > 0 && id <= (size)) {
							found = true;
							std::cout << "\t* Please enter data for each field.";
							std::string newData = "";
							for (size_t k = 0; k < this->storageManager->getTable(databaseName, tableName)->columnNames.size(); k++) {
								std::cout << "\t* " << this->storageManager->getTable(databaseName, tableName)->columnNames[k] << ':';
								newData.clear();
								std::cin >> newData;
								this->storageManager->getTable(databaseName, tableName)->records[id]->dataFields[k] = newData;
							}
						}
					}
				}
			}
		}
	}

	void ConsoleUI::uploadFile() const
	{
		std::cout << "\t\t\t UPLOAD FILE" << std::endl;
		std::string fileName = "";
		auto fileHelper = std::make_unique<FileIOHelper>();

		std::cout << "\t*Please enter the name of a file to upload:" << std::endl;
		std::cout << "\t*";
		std::cin >> fileName;

		try {
			this->parser->parseConfig(fileHelper->loadStorageText(fileName));
			std::cout << "\t*Data successfully loaded." << std::endl;
		}
		catch (const std::runtime_error &e) {
			std::cout << "\t*" << e.what() << std::endl;
		}
	}

	void ConsoleUI::printDatabases() const
	{
		if (this->storageManager->getStorage()->databases.size() > 0) {
			std::cout << "\t*Your storage currently has these databases:" << std::endl;
			std::cout << "\t-----------------------------------------------" << std::endl;
			for (size_t i = 0; i < this->storageManager->getStorage()->databases.size(); i++) {
				std::cout << "\t<Database>: [" << this->storageManager->getStorage()->databases[i]->databaseName << "]\n";
				std::cout << this->storageManager->getStorage()->databases[i];
			}
			std::cout << "\t-----------------------------------------------" << std::endl;
		}
		else {
			std::cout << "\t-----------------------------------------------" << std::endl;
			std::cout << "\t*You don't have any databases." << std::endl;;
			std::cout << "\t-----------------------------------------------" << std::endl;
		}
		return;
	}

	void ConsoleUI::printTablesFromDatabase(std::string databaseName) const
	{
		if (this->storageManager->getDatabase(databaseName)->tables.size() > 0) {
			std::cout << "\t*Database " << databaseName << " has theese tables:" << std::endl;
			std::cout << "\t-----------------------------------------------" << std::endl;
			for (size_t i = 0; i < this->storageManager->getDatabase(databaseName)->tables.size(); i++) {
				std::cout << this->storageManager->getTable(databaseName, this->storageManager->getDatabase(databaseName)->tables[i]->tableName);
			}
			std::cout << "\t-----------------------------------------------" << std::endl;
		}
		else {
			std::cout << "\t-----------------------------------------------" << std::endl;
			std::cout << "\t*You don't have any tables." << std::endl;;
			std::cout << "\t-----------------------------------------------" << std::endl;
		}
		return;
	}
	void ConsoleUI::printRecordsFromTable(std::string databaseName, std::string tableName) const
	{
		for (size_t i = 0; i < this->storageManager->getStorage()->databases.size(); i++) {
			if (this->storageManager->getStorage()->databases[i]->databaseName == databaseName) {
				for (size_t k = 0; k < this->storageManager->getStorage()->databases[i]->tables.size(); k++) {
					if (this->storageManager->getStorage()->databases[i]->tables[k]->tableName == tableName) {
						for (size_t j = 0; j < this->storageManager->getStorage()->databases[i]->tables[k]->records.size(); j++) {
							std::cout << this->storageManager->getStorage()->databases[i]->tables[k]->records[j];
						}
					}
				}
			}
		}
	}


}

