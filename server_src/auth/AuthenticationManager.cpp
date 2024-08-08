// Copyright 2021
// Author: Ayvar Aleksiev

#include "AuthenticationManager.h"
#include "../database/SQLiteDBConnection.h"
#include "User.h"


namespace Notes {

AuthenticationManager*
AuthenticationManager::getInstance()
{
   static std::unique_ptr<AuthenticationManager> sManagerPtr(
      std::make_unique<AuthenticationManager>());
   return sManagerPtr.get();
}

void
AuthenticationManager::createUser(std::string const& username, std::string const& password)
{
   if (username.empty() || password.empty() || username.length() > 50 || password.length() > 50) {
      throw Utils::HttpException(restinio::status_bad_request(), "Invalid username/password input.");
   }
   if (_dbService->getUserInfo(username)) {
      throw Utils::HttpException(restinio::status_bad_request(), "User already exists.");
   }
   if (!_dbService->createUser(username, Utils::generatePasswordHash(password))) {
      throw Utils::HttpException(restinio::status_internal_server_error(),
         "'" + username + "'" + " cannot be inserted into database.");
   }
}

bool
AuthenticationManager::authenticateUser(std::string const& username, std::string const& password)
{
   std::optional<std::tuple<User, int>> userInfo = _dbService->getUserInfo(username);
   if (userInfo) {
      User const& user = std::get<0>(*userInfo);
      return user.comparePasswordHash(Utils::generatePasswordHash(password));
   }
   return false;
}

void
AuthenticationManager::changeUserPassword(std::string const& username, std::string const& oldPassword, std::string const& newPassword)
{
   if (newPassword.empty() || newPassword.length() > 50) {
      throw Utils::HttpException(restinio::status_bad_request(), "New password input is invalid.");
   }
   bool succeed = 
      _dbService->changeUserPassword(
         username, 
         Utils::generatePasswordHash(oldPassword), 
         Utils::generatePasswordHash(newPassword));
   if(!succeed) {
      throw Utils::HttpException(restinio::status_bad_request(), "Invalid username or password.");
   }
}

bool
AuthenticationManager::userExist(std::string const& username)
{
   std::optional<std::tuple<User, int>> userInfo = _dbService->getUserInfo(username);
   if (userInfo) {
      return true;
   }
   return false;
}

void
AuthenticationManager::deleteUser(std::string const& username, std::string const& password)
{
   assert(!username.empty() && !password.empty());
   _dbService->deleteUser(username, Utils::generatePasswordHash(password));
}

} //namspace Notes
