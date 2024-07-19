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
   { }

   std::string getUserName() const
   {
      return _username;
   }
   void changePassword(std::string const& newPassword)
   {
      if (newPassword == _password) {
         throw Utils::HttpException(restinio::status_bad_request(), "Cannot use the same password as the current one.");
      }
      _password = newPassword;
   }
   bool comparePasswordHash(std::string const& otherPasswordHash) const
   {
      return _password == otherPasswordHash;
   }

private:

   std::string _username;
   
   /// Hashed password.
   std::string _password;
};

} // namespace Notes
