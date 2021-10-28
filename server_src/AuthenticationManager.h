// Copyright 2021
// Author: Ayvar Aleksiev

#pragma once

#include "User.h"
#include "utils/Crypto.h"

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
      assert(!password.empty());
      // todo should i check here for empty password.
      if (_users.contains(User(username, ""))) {
         // todo htrow exception. User already exists.
         // // Add log message
         // Should I remember users that have deleted there accounts.
      }
      _users.emplace(username, Utils::generatePasswordHash(password));
   }
   
   // todo return JWT?
   bool authenticateUser(std::string const& username, std::string const& password)
   {
      auto userIt = _users.find(User(username, ""));

      return (userIt != _users.end()) && (*userIt).comparePasswordHash(Utils::generatePasswordHash(password));
   }

   void deleteUser(std::string const& username, std::string const& password)
   {
      std::scoped_lock writeLock(_mutex);
      assert(!password.empty());
      
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