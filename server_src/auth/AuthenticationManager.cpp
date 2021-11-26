// Copyright 2021
// Author: Ayvar Aleksiev

#include "AuthenticationManager.h"


namespace Notes {

AuthenticateionManager*
AuthenticateionManager::getInstance()
{
   static std::unique_ptr<AuthenticateionManager> sManagerPtr(
      std::make_unique<AuthenticateionManager>());
   return sManagerPtr.get();
}

void
AuthenticateionManager::createUser(std::string const& username, std::string const& password)
{
   std::scoped_lock writeLock(_mutex);
   if (username.empty() || password.empty() || username.length() > 50 || password.length() > 50) {
      throw Utils::HttpException(restinio::status_bad_request(), "Invalid username/password input.");
   }

   if (_users.contains(User(username, ""))) {
      throw Utils::HttpException(restinio::status_bad_request(), "User already exists");
   }
   _users.emplace(username, Utils::generatePasswordHash(password));
}

bool
AuthenticateionManager::authenticateUser(std::string const& username, std::string const& password)
{
   assert(!username.empty() && !password.empty());
   auto userIt = _users.find(User(username, ""));

   return (userIt != _users.end()) && (*userIt).comparePasswordHash(Utils::generatePasswordHash(password));
}

void
AuthenticateionManager::changeUserPassword(std::string const& username, std::string const& oldPassword, std::string const& newPassword)
{
   std::scoped_lock writeLock(_mutex);
   if (!authenticateUser(username, oldPassword)) {
      throw Utils::HttpException(restinio::status_bad_request(), "Invalid username or password.");
   }
   if (newPassword.empty() || newPassword.length() > 50) {
      throw Utils::HttpException(restinio::status_bad_request(), "New password input is invalid.");
   }

   auto userIt = _users.find(User(username, ""));
   assert(userIt != _users.end());
   _users.erase(userIt);
   _users.emplace(username, Utils::generatePasswordHash(newPassword));
}

bool
AuthenticateionManager::userExist(std::string const& username)
{
   auto userIt = _users.find(User(username, ""));

   return userIt != _users.end();
}

void
AuthenticateionManager::deleteUser(std::string const& username, std::string const& password)
{
   assert(!username.empty() && !password.empty());
   std::scoped_lock writeLock(_mutex);

   auto userIt = _users.find(User(username, ""));
   if ((userIt != _users.end()) && (*userIt).comparePasswordHash(Utils::generatePasswordHash(password))) {
      _users.erase(userIt);
   }
}

} //namspace Notes
