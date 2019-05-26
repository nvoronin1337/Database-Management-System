#pragma once
#ifndef PARSER
#define PARSER

#include "StorageManager.h"
#include <map>

namespace dbms {
	class Parser {
	private:
		StorageManager* storageManager;
		std::vector<char> dmbsConfig;
	public:
		Parser() = default;
		Parser(StorageManager* storageManager, std::vector<char> congif);
		~Parser();
		
		void parseConfig() const;
		void parseConfig(std::vector<char> config) const;
		
		std::string getKey(size_t& index, std::vector<char> config) const;
		std::string getValue(size_t& index, std::vector<char> config) const;
		std::vector<std::string> getColumnNames(size_t& index, std::vector<char> config) const;
		std::vector<std::string> getRecordFields(size_t& index, size_t size, std::vector<char> config) const;
		void skipRecordIDLine(size_t& index, std::vector<char> config) const;

		void setStorageManager(StorageManager* manager) {
			this->storageManager = manager;
		}

		void setConfig(std::vector<char> config) {
			this->dmbsConfig = config;
		}

		/*! \method parseUserCredentials(vector<char> unparsed, map<string,string> &credentials)
		 *
		 */
		void parseUserCredentials(std::vector<char> unparsed, std::map<std::string, std::string> &credentials);
	};
}
#endif // !PARSER 