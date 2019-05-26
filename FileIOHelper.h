#pragma once
#ifndef FILEIOHELPER
#define FILEIOHELPER


#include "Storage.h"
#include "Parser.h"
#include "picosha2.h"


#include <fstream>
#include <iostream>

namespace dbms {
	class FileIOHelper {
		
	private:
		const std::string storageFileText = "storage.txt";
		const std::string storageFileBinary = "storage.dat";
	public:
		FileIOHelper() = default;

		/** HW Using Streams and Exceptions
		*	Throws std::invalid_argument("File doesn't exist") if there is no file "existingTableFile.txt" in project directory.
		*	Reads the file if no exception is thrown.
		*/
		FileIOHelper(std::string existingTableFile);	
		
		// To Be Deleted or converted to !test!
		void printResult() noexcept {
			std::vector<char> result = loadStorageText("storage.txt");
			for (size_t i = 0; i < result.size(); i++) {
				std::cout << result[i];
			}
		}

		// Method for reading any txt file
		std::vector<char> loadStorageText(std::string fileName) const;

		void saveStorageText(Storage* storage, std::string fileName) const;
		void saveStorageBinary(Storage* storage, std::string fileName) const;

		// for logger
		void saveUserCredentials(std::string credentials, std::string fileName) const;
	};
}
#endif