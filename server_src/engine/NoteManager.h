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

   void addUserNoteBoard(std::string const& user);

   std::shared_ptr<NoteBoard> getUserNoteBoard(std::string const& user);

private:
   // Users' noteboards. [usersname -> noteboard]
   std::unordered_map<std::string, std::shared_ptr<NoteBoard>> _noteboards;
   
   /// Mutex
   mutable std::shared_mutex  _mutex;

};

} // namespace Notes