#pragma once
#ifndef GUI_USER
#define GUI_USER

#include <string>

// From DBMS
#include <StorageManager.h>
#include <FileIOHelper.h>
#include <Parser.h>


class GuiUser
{
public:
	GuiUser(std::string username);
	~GuiUser();

	void loadAndParseStorage();

	std::string getUsername() const;
	dbms::StorageManager* getStorageManager() const;

private:
	std::string username;
	dbms::Storage* storage;
	dbms::StorageManager *storageManager;
	dbms::Parser* parser;
};
#endif
