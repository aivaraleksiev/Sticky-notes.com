// Copyright 2021
// Author: Ayvar Aleksiev

#pragma once

#include "NoteBoard.h"
#include "../utils/HttpException.h"
#include <map>
#include <memory>
#include <string>

namespace Notes {

class SQLiteDBConnection;

class NoteManager
{
public:

   // Access to the singleton instance of this class.
   static NoteManager* getInstance();

   std::shared_ptr<NoteBoard> getUserNoteBoard(std::string const& user);

   void setDbConnection(std::shared_ptr<SQLiteDBConnection> const& dbService) {
      _dbService = dbService;
   }
   std::vector<std::shared_ptr<Note>> getUserNotes(std::string const& username);

   std::vector<std::shared_ptr<Note>> getUserNotes(
      std::string const& username,
      std::string const& title,
      std::string const& text,
      Color noteColor = Color::invalid);

   std::shared_ptr<Note> getUserNoteByUID(std::string const& username, UID uid);

   bool addNotes(std::string const& username, std::vector<std::shared_ptr<Note>> const& newNotes);
private:
   std::optional<int> getUserID(std::string const& username);

   std::shared_ptr<SQLiteDBConnection> _dbService {nullptr};

   // Users' noteboards. [usersname -> noteboard]
   std::unordered_map<std::string, std::shared_ptr<NoteBoard>> _noteboards;
   
   /// Mutex
   mutable std::shared_mutex  _mutex;

};

} // namespace Notes
