#pragma once
#include "IDatabase.h"
#include <string>
#include <bsoncxx/builder/stream/document.hpp>
#include <mongocxx/options/create_collection.hpp>
#include <bsoncxx/json.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/stdx.hpp>
#include <mongocxx/uri.hpp>
using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_array;
using bsoncxx::builder::basic::make_document;
using std::string;
const string DB_NAME = "triviaDB";

class MongoDatabase : public IDatabase
{
public:
	MongoDatabase();
	~MongoDatabase();

	//Open&Close functions 
	bool open() override;
	bool close() override;

	//User functions
	int doesUserExist(const string username) override;
	int doesPasswordMatch(const string username, const string password) override;
	int addNewUser(const  User& user) override;

private:
	mongocxx::database db;
	mongocxx::uri uri;
	mongocxx::client client;
};

