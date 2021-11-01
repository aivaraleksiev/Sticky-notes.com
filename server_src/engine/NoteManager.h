// Copyright 2021
// Author: Ayvar Aleksiev

#pragma once

#include "NoteBoard.h"
#include "../utils/HttpException.h"
#include <map>
#include <memory>
#include <string>

namespace Notes {

class NoteManager
{
public:

   // Access to the singleton instance of this class.
   static NoteManager* getInstance();

   void addUserNoteBoard(std::string const& user)
   {  
      //todo assert in release
      _noteboards.emplace( user, std::make_shared<NoteBoard>());
      
   }

   std::shared_ptr<NoteBoard> getUserNoteBoard(std::string const& user)
   {
      auto it = _noteboards.find(user);
      if (it == _noteboards.end()) {
         throw Utils::HttpException(restinio::status_not_found(), "User not found.");
      }
      return it->second;
   }

private:
   // [usersname -> noteboards]
   // Users' noteboards.
   std::unordered_map<std::string, std::shared_ptr<NoteBoard>> _noteboards;
};

} // namespace Notes