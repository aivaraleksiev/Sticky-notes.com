// Copyright 2021
// Author: Ayvar Aleksiev

#pragma once

#include <shared_mutex>

namespace Notes {

class User
{
public:
   User(std::string const& username,
        std::string const& hashedPassword)
      : _username(username),
        _password(hashedPassword)
   {
      // todo add username length limitations somewhere.
   }

   std::string getUserName() const
   {
      std::shared_lock<std::shared_mutex> readLock(_mutex);
      return _username;
   }
   void changePassword(std::string const& newPassword)
   {
      std::scoped_lock writeLock(_mutex);
      if (newPassword == _password) {
         // throw exception;
      }
      _password = newPassword;
   }
   bool comparePasswordHash(std::string const& otherPasswordHash) const
   {
      std::shared_lock<std::shared_mutex> readLock(_mutex);
      return _password == otherPasswordHash;
   }

private:

   std::string _username;
   
   /// Hashed password.
   std::string _password;

   /// Mutex
   mutable std::shared_mutex  _mutex;
};

} // namespace Notes
