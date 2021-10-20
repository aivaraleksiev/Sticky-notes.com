// Copyright 2021
// Author: Ayvar Aleksiev

#pragma once

#include "utils/Utils.h"

using Notes::Utils::UID;

namespace Notes {

class User
{
public:
   User(std::string const& username,
        std::string const& hashedPassword,
        UID id = Utils::UIDGenerator::generateUID())
      : _username(username),
        _password(hashedPassword),
        _id(id)
   {}
   void setUID(UID newId)
   {
      _id = newId;
   }
   UID getUID()
   {
      return _id;
   }
   std::string GetUserName()
   {
      return _username;
   }
   void changePassword(std::string const& newPassword)
   {
      if (newPassword == _password) {
         // throw exception;
      }
      _password = newPassword;
   }
private:
   std::string _username;
   
   /// Hashed password.
   std::string _password;

   UID _id;
};

} // namespace Notes
