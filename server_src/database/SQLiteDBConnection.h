// Copyright 2024-
// Author: Ayvar Aleksiev

#pragma once
#include <sqlite3.h>
#include <string>
#include <vector>
#include <optional>
#include <memory>
#include <shared_mutex>
#include <tuple>
#include "../utils/Logger.h"
#include "../engine/Note.h"
#include "../auth/User.h"

namespace Notes {

class SQLiteDBConnection
{
public:
   // Factory method to create instances.
   static std::shared_ptr<SQLiteDBConnection> createInstance(std::string const& dbName) {
      std::shared_ptr<SQLiteDBConnection> dbService(new SQLiteDBConnection(dbName));
      dbService->initializeDatabase();
      return dbService;
   }

   ~SQLiteDBConnection() = default;
   SQLiteDBConnection(SQLiteDBConnection const&) = delete;
   SQLiteDBConnection& operator=(SQLiteDBConnection const&) = delete;
   
   bool initializeDatabase();
   bool clearDatabase();
   /*
    * Users Table
    */
   bool createUser(std::string const& username, std::string const& password_hash);
   bool deleteUser(std::string const& username, std::string const& password_hash);
   bool changeUserPassword(
      std::string const& username,
      std::string const& oldPassword,
      std::string const& newPassword);
   std::optional<std::tuple<User, int>> getUserInfo(std::string const& username);
   /*
    * Notes Table
    */
   bool addNotes(int64_t user_id, std::vector<std::shared_ptr<Note>> notes);
   std::vector<std::string> getUserNoteUIDs(int user_id);
   std::vector<std::shared_ptr<Note>> getUserNotes(int user_id);
   std::vector<std::shared_ptr<Note>> getUserNotes(
      int user_id,
      std::string const& title,
      std::string const& text,
      Color noteColor = Color::invalid);
   std::shared_ptr<Note> getUserNoteByUID(int user_id, int uid);
   bool updateNoteByUid(
      int user_id,
      int uid,
      const std::optional<std::string>& title,
      const std::optional<std::string>& text,
      const std::optional<Color>& noteColor);
   bool deleteNoteByUid(int user_id, int uid);

private:
   // Constructor
   SQLiteDBConnection(std::string const& dbName)
      : _dbName(dbName),
      _logger(Utils::createLogger("DbConnection", spdlog::level::trace))
   {}

   bool executeQuery(std::string_view query);
   // Execute parameterized queries.
   bool executeParamQuery(std::string_view query, std::vector<std::string> const& queryParams);

   bool beginTransaction();
   bool commitTransaction();
   bool rollbackTransaction();
   std::string sanitizeInput(std::string const& input);
private:
   std::string _dbName;
   
   using DBDeleter = decltype(
      [](sqlite3* db) { 
         if (db) { 
            sqlite3_close(db); 
         } 
      });
   std::unique_ptr <sqlite3, DBDeleter> _dbPtr { nullptr };

   mutable std::shared_mutex _userTableMutex;
   mutable std::shared_mutex _noteTableMutex;

   std::shared_ptr<spdlog::logger> _logger;
};

} // namespace Notes
