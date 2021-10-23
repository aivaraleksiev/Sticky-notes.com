// Copyright 2021
// Author: Ayvar Aleksiev

#pragma once

#include "NoteBoard.h"
#include <map>
#include <string>

namespace Notes {

class NoteManager
{
public:

   // Access to the singleton instance of this class.
   static NoteManager* getInstance();

   void addUserNoteBoard(std::string const& user)
   {      
      _noteboards.emplace( user, NoteBoard());
   }

   NoteBoard& getUserNoteBoard(std::string const& user)
   {
      auto it = _noteboards.find(user);
      return it->second;
   }

private:
   // [usersname -> noteboards]
   // Users' noteboards.
   std::map <std::string, NoteBoard> _noteboards;
};

} // namespace Notes