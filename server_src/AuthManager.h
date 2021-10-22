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

/// Class handling authenticaion and ahthorization.
class AuthManager
{
public:

   AuthManager() = default;
   ~AuthManager() = default;
   AuthManager(AuthManager const&) = delete;
   AuthManager& operator=(AuthManager const&) = delete;
   void addUser(std::string const& username, std::string const& password)
   {
      std::scoped_lock writeLock(_mutex);
      assert(!password.empty());
      if (_users.contains(User(username, ""))) {
         // todo htrow exception. User already exists.
         // Should I remember users that have deleted there accounts.
      }
      // todo should i check here for empty password.
      _users.emplace(username, Utils::generatePasswordHash(password));
   }
   
   // todo return JWT
   bool authenticateUser(std::string const& username, std::string const& password)
   {
      auto user = _users.find(User(username, ""));
      if (user != _users.end()) {
         return (*user).comparePasswordHash(Utils::generatePasswordHash(password));
      }
      return false;
   }
private:

   using equalFunc =
      decltype([](User const& left, User const& right) {return left.getUserName() == right.getUserName(); });
   using hashFunc =
      decltype([](User const& obj) { std::hash<std::string> string_hash; return string_hash(obj.getUserName()); });

   std::unordered_set<User, hashFunc, equalFunc> _users;
   //std::vector<User> _users;

   /// Mutex
   mutable std::shared_mutex  _mutex;
};

} // namespace Notes