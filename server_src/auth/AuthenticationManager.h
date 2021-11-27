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

   void createUser(std::string const& username, std::string const& password);
   
   bool authenticateUser(std::string const& username, std::string const& password);

   void changeUserPassword(std::string const& username, std::string const& oldPassword, std::string const& newPassword);

   bool userExist(std::string const& username);

   void deleteUser(std::string const& username, std::string const& password);

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