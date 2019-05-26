#include "GuiUser.h"


GuiUser::GuiUser(const std::string username)
{
	this->username = username;

	loadAndParseStorage();	// read the data into the storage in user's storagemanager
}

GuiUser::~GuiUser()
{
	delete(parser);
	parser = nullptr;

	delete(storageManager);
	storageManager = nullptr;

	delete(storage);
	storage = nullptr;
}

void GuiUser::loadAndParseStorage()
{
	using namespace dbms;
	this->storage = new Storage();
	this->storageManager = new StorageManager(this->storage);


	auto fileHelper = std::make_unique<FileIOHelper>();

	try {
		this->parser = new Parser(this->storageManager, fileHelper->loadStorageText(this->username + ".txt"));
		this->parser->parseConfig();
	}
	catch (std::exception ex) {
		this->parser = new Parser();
		this->parser->setStorageManager(this->storageManager);
	}

}

std::string GuiUser::getUsername() const
{
	return this->username;
}

dbms::StorageManager* GuiUser::getStorageManager() const
{
	return this->storageManager;
}
