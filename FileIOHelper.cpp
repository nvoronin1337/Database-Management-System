#include "pch.h"
#include "FileIOHelper.h"

namespace dbms {
	FileIOHelper::FileIOHelper(std::string existingTableFile)
	{
		std::fstream fileIn(existingTableFile, std::ios::in);
		if (fileIn.is_open()) {
			// read file, give vector<char> to parser so parser can add the table.
			std::vector<char> input = loadStorageText(existingTableFile);
			// now just give it to parser.
			auto parser = new Parser();
		}
		else {
			throw std::invalid_argument("File doesn't exist");
		}
	}

	// INPUT
	std::vector<char> FileIOHelper::loadStorageText(std::string fileName) const {
		std::vector<char> input = std::vector<char>();
		std::fstream fileIn(fileName, std::ios::in);
		
		if (fileIn.fail()) {
			fileIn.close();
			throw std::runtime_error("Unable to open the file. Please enter a valid file name in this format: \"example.txt\"");
		}
		char ch;
		if (fileIn.is_open()) {
			while (fileIn >> std::skipws >> ch) {
				input.push_back(ch);
			}
		}
		
		if (!fileIn.eof()) {
			fileIn.close();
			throw std::runtime_error("Error reading the file.");
		}
		fileIn.close();
		return input;
	}

	// OUTPUT
	void FileIOHelper::saveStorageText(Storage * storage, std::string fileName) const
	{
		std::fstream file(fileName, std::ios::out);
		if (file.is_open()) {
			file << storage;
		}
		file.close();
	}
	
	void FileIOHelper::saveStorageBinary(Storage * storage, std::string fileName) const	// not working	need to fix
	{
		std::fstream file(fileName, std::ios::out | std::ios::binary);
		if (file.is_open()) {
			std::string str_to_write = storage->to_string();
			file.write(str_to_write.data(), str_to_write.size());
		}
		file.close();
	}

	void FileIOHelper::saveUserCredentials(std::string credentials, std::string fileName) const
	{
		std::fstream file(fileName, std::ios::out);
		if (file.is_open()) {
			file << credentials;
		}
		file.close();
	}
}