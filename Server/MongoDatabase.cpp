#include "MongoDatabase.h"

/// <summary>
/// c`tor for mongo database
/// </summary>
MongoDatabase::MongoDatabase()
{
	if (!this->open())
	{
		throw std::exception("Cannot open the database");
	}
}

/// <summary>
/// open mongo database
/// </summary>
/// <returns> if the database was opened ssucessfully or not</returns>
bool MongoDatabase::open()
{
	this->uri = mongocxx::uri("mongodb://localhost:27017");
	try
	{
		this->client = mongocxx::client{ uri };//connect to the server
		this->db = this->client["triviaDB"];
		if (!this->db.has_collection("USERS"))
		{
			this->db.create_collection("USERS");
		}
		return true;
	}
	catch (...)
	{
		return false;
	}
}

/// <summary>
/// close the server
/// </summary>
/// <returns>false</returns>
bool MongoDatabase::close()//the database is closed automatically
{
	return false;
}

/// <summary>
/// check if a user exists
/// </summary>
/// <param name="username">the username to check for</param>
/// <returns> true or false</returns>
int MongoDatabase::doesUserExist(const string username)
{
	// Build the query
	auto doc = make_document(kvp("username", username));

	// Execute the query
	auto cursor = this->db["USERS"].find(doc.view());
	if (cursor.begin() != cursor.end())//if there were found
	{
		return USER_EXIST;
	}
	return USER_NOT_EXIST;
}

int MongoDatabase::doesPasswordMatch(const string username, const string password)
{
	auto doc = make_document(kvp("username", username), kvp("password", password));
	// Execute the query
	auto cursor = this->db["USERS"].find(doc.view());

	if (cursor.begin() != cursor.end())// if there were found
	{
		return PASSWORD_MATCH;
	}
	return PASSWORD_NOT_MATCH;
}

int MongoDatabase::addNewUser(const User& user)
{
	// Document with username, password, and email fields
	auto doc = make_document(kvp("email", user.getEmail()),
		kvp("username", user.getUsername()),
		kvp("password", user.getPassword()),
		kvp("id",user.getId()),
		kvp("address",user.getAddress()),
		kvp("birth date",user.getBornDate()),
		kvp("phone number",user.getPhoneNum()));
	try
	{
		auto insert_one_result = this->db["USERS"].insert_one(doc.view());
		return OK_CODE;
	}
	catch (...)
	{
		return ERROR_CODE;
	}
}
