// Copyright 2021
// Author: Ayvar Aleksiev

#pragma once

#include "../utils/HttpException.h"

#include "private/User.h"
#include "private/Crypto.h"

#include <algorithm>
#include <unordered_set>
#include <shared_mutex>
#include <string>
#include <cassert>

namespace Notes {

/// Class handling authenticaion.
class AuthenticateionManager
{
public:

   // Access to the singleton instance of this class.
   static AuthenticateionManager* getInstance();

   void createUser(std::string const& username, std::string const& password)
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
   
   bool authenticateUser(std::string const& username, std::string const& password)
   {
      assert(!username.empty() && !password.empty());
      auto userIt = _users.find(User(username, ""));

      return (userIt != _users.end()) && (*userIt).comparePasswordHash(Utils::generatePasswordHash(password));
   }

   bool userExist(std::string const& username)
   {
      auto userIt = _users.find(User(username, ""));

      return userIt != _users.end();
   }

   void deleteUser(std::string const& username, std::string const& password)
   {
      assert(!username.empty() && !password.empty());
      std::scoped_lock writeLock(_mutex);
      
      auto userIt = _users.find(User(username, ""));
      if ((userIt != _users.end()) && (*userIt).comparePasswordHash(Utils::generatePasswordHash(password))) {
         _users.erase(userIt);
      }
   }

private:

   using equalFunc =
      decltype([](User const& left, User const& right) {return left.getUserName() == right.getUserName(); });
   using hashFunc =
      decltype([](User const& obj) { std::hash<std::string> stringHash; return stringHash(obj.getUserName()); });

   std::unordered_set<User, hashFunc, equalFunc> _users;

   /// Mutex
   mutable std::shared_mutex  _mutex;
};

} // namespace Notes