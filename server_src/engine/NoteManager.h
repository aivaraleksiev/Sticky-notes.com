// Copyright 2021 - 2024
// Author: Ayvar Aleksiev

#pragma once

#include "Note.h"
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

   bool updateNoteByUid(
      std::string const& username, 
      int uid,
      const std::optional<std::string>& title,
      const std::optional<std::string>& text,
      const std::optional<Color>& noteColor);

   bool deleteNoteByUid(std::string const& username, int uid);

   bool deleteNotesForUser(std::string const& username);

private:
   std::optional<int> getUserID(std::string const& username);

   std::shared_ptr<SQLiteDBConnection> _dbService {nullptr};
};

} // namespace Notes
