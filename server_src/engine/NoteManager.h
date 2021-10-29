// Copyright 2021
// Author: Ayvar Aleksiev

#pragma once

#include "NoteBoard.h"
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
      _noteboards.emplace( user, std::make_shared<NoteBoard>());
   }

   std::shared_ptr<NoteBoard> getUserNoteBoard(std::string const& user)
   {
      auto it = _noteboards.find(user);
      if (it == _noteboards.end()) {
         // todo throw 
      }
      return it->second;
   }

private:
   // [usersname -> noteboards]
   // Users' noteboards.
   std::map <std::string, std::shared_ptr<NoteBoard>> _noteboards;
};

} // namespace Notes