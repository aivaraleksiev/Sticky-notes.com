// Copyright 2021
// Author: Ayvar Aleksiev

#pragma once

#include "../../utils/HttpException.h"
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
   }
   User(User const& other)
   {
      std::shared_lock<std::shared_mutex> readLock(other._mutex);
      _username = other._username;
      _password = other._password;
   }
   User& operator=(User const& other)
   {
      std::unique_lock<std::shared_mutex> writeLock(_mutex, std::defer_lock);
      std::shared_lock<std::shared_mutex> readLock(other._mutex, std::defer_lock);
      std::lock(writeLock, readLock);
      if (this != &other) {
         _username = other._username;
         _password = other._password;
      }
      return *this;
   }
   User(User&& other) noexcept
   {
      std::shared_lock<std::shared_mutex> readLock(other._mutex);
      _username = std::move(other._username);
      _password = std::move(other._password);
   }

   User& operator=(User && other) noexcept
   {
      std::unique_lock<std::shared_mutex> writeLock(_mutex, std::defer_lock);
      std::shared_lock<std::shared_mutex> readLock(other._mutex, std::defer_lock);
      std::lock(writeLock, readLock);

      if (this != &other) {
         _username = std::move(other._username);
         _password = other._password;
      }
      return *this;
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
         throw Utils::HttpException(restinio::status_bad_request(), "Cannot use the same password as the current one.");
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
