#include "pch.h"
#include "Parser.h"

dbms::Parser::Parser(StorageManager * storageManager, std::vector<char> config)
{	
	this->storageManager = storageManager;
	this->dmbsConfig = config;
}

dbms::Parser::~Parser()
{
	delete(this->storageManager);
	this->storageManager = nullptr;
}


void dbms::Parser::parseConfig() const
{
	size_t index = 0;
	std::string key = "";
	std::string database = "";
	std::string table = "";
	std::vector<std::string> columnNames = std::vector<std::string>();
	std::vector<std::string> recordFields = std::vector<std::string>();

	while (index < this->dmbsConfig.size()) {
		key = getKey(index, this->dmbsConfig);
		if (key == "Database") {
			index++;	// to skip ':'
			database = getValue(index, this->dmbsConfig);
			this->storageManager->addDatabase(database);
		}
		else if (key == "Table") {
			index++;	// to skip ':'
			table = getValue(index, this->dmbsConfig);
			columnNames = getColumnNames(index, this->dmbsConfig);
			this->storageManager->addTableToDatabase(database, table, columnNames);
		}
		else if (key == "RecordID") {
			index++;	// to skip ':'
			skipRecordIDLine(index, this->dmbsConfig);
		}
		else if (key == "data") {
			recordFields = getRecordFields(index, columnNames.size(), this->dmbsConfig);
			this->storageManager->addRecordToTableInDatabase(database, table, recordFields);
			index--;
		}
	}
}

void dbms::Parser::parseConfig(std::vector<char> config) const
{
	size_t index = 0;
	std::string key = "";
	std::string database = "";
	std::string table = "";
	std::vector<std::string> columnNames = std::vector<std::string>();
	std::vector<std::string> recordFields = std::vector<std::string>();

	while (index < config.size()) {
		key = getKey(index, config);
		if (key == "Database") {
			index++;	// to skip ':'
			database = getValue(index, config);
			this->storageManager->addDatabase(database);
		}
		else if (key == "Table") {
			index++;	// to skip ':'
			table = getValue(index, config);
			columnNames = getColumnNames(index, config);
			this->storageManager->addTableToDatabase(database, table, columnNames);
		}
		else if (key == "RecordID") {
			index++;	// to skip ':'
			skipRecordIDLine(index, config);
		}
		else if (key == "data") {
			recordFields = getRecordFields(index, columnNames.size(), config);
			this->storageManager->addRecordToTableInDatabase(database, table, recordFields);
			index--;
		}
	}
}

std::string dbms::Parser::getKey(size_t& index, std::vector<char> config) const
{
	index++; //to skip '<'
	char ch = config.at(index);
	std::string key = "";
	if (index < config.size()) {
		while (ch != '>' ) {
			key.push_back(ch);
			index++;
			ch = config.at(index);
			//std::cout << ch << std::endl;
		}
		index++; //to skip '>'
	}
	return key;
}

std::string dbms::Parser::getValue(size_t & index, std::vector<char> config) const
{
	index++; //to skip '['
	char ch = config.at(index);
	std::string value = "";
	if (index < config.size()) {
		while (ch != ']') {
			value.push_back(ch);
			index++;
			ch = config.at(index);
			//std::cout << ch << std::endl;
		}
		index++; //to skip ']'
	}
	return value;
}

std::vector<std::string> dbms::Parser::getColumnNames(size_t & index, std::vector<char> config) const
{
	std::vector<std::string> colNames = std::vector<std::string>();
	index++; //to skip '{'
	char ch = config.at(index);
	std::string column = "";
	if (index < config.size()) {
		while (ch != '}') {
			ch = config.at(index);
			//std::cout << "col ch: " << ch << std::endl;
			if (ch == ',') {
				colNames.push_back(column);
				column.clear();
			}
			else {
				column.push_back(ch);
			}
			index++;
		}
		column.pop_back();
		colNames.push_back(column);
	}
	return colNames;
}

std::vector<std::string> dbms::Parser::getRecordFields(size_t & index, size_t size, std::vector<char> config) const
{
	std::vector<std::string> fields = std::vector<std::string>();
	std::string field = "";
	char ch = config.at(index);
	// read data for every column
	for (size_t i = 0; i < size; i++) {
		while (ch != '[') {
			index++;
			ch = config.at(index);
		}
		index++;	// to skip '['
		while (ch != ']') {
			ch = config.at(index);
			field.push_back(ch);
			index++;
		}
		field.pop_back();
		fields.push_back(field);
		field.clear();
		index++;	// to skip ']'
	}
	return fields;
}

void dbms::Parser::skipRecordIDLine(size_t & index, std::vector<char> config) const
{
	index++; //to skip '['
	char ch = config.at(index);
	//std::cout << "rec id line 111: " << ch << std::endl;
	if (index < config.size()) {
		while (ch != ']') {
			//std::cout << "rec id line: " << ch << std::endl;
			index++;
			ch = config.at(index);
		}
		index++; //to skip ']'
	}

	return;
}

void dbms::Parser::parseUserCredentials(std::vector<char> unparsed, std::map<std::string, std::string>& credentials)
{
	size_t index = 0;
	std::string userName = "";
	std::string password = "";
	char ch;

	while (index < unparsed.size()) {
		ch = unparsed[index];
		if (ch == '<') {
			index++;
			ch = unparsed[index];
			while (ch != ':') {
				userName.push_back(ch);
				index++;
				ch = unparsed[index];
			}
			index++;
			ch = unparsed[index];
			while (ch != '>') {
				password.push_back(ch);
				index++;
				ch = unparsed[index];
			}
		}
		index++;
		credentials[userName] = password;
		userName.clear();
		password.clear();
	}
}
