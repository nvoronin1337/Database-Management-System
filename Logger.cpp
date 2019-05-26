#include "pch.h"
#include "Logger.h"
#include "Registrator.h"
#include "Login.h"


dbms::Logger::Logger()
{
	auto fileHelper = std::make_unique<FileIOHelper>();
	std::vector<char> unparsedContents = fileHelper->loadStorageText(this->file);	// read the file
	
	auto parser = std::make_unique<Parser>();
	parser->parseUserCredentials(unparsedContents, this->userCredentials);
}

dbms::Logger::~Logger()
{
	auto fileHelper = std::make_unique<FileIOHelper>();
	fileHelper->saveUserCredentials(this->to_string(), this->file);	// save current credentials
}


std::string dbms::Logger::to_string() const
{
	std::string mapContent = "";
	for (auto& x : this->userCredentials) {
		mapContent.append("<" + x.first + ":" + x.second + ">");
	}
	return mapContent;
}

std::ostream & dbms::operator<<(std::ostream & output, const Logger * logger)
{
	std::string mapContent = "";
	for (auto& x : logger->userCredentials) {
		mapContent.append("<" + x.first + ":" + x.second + ">");
	}
	output << mapContent;
	return output;
}
