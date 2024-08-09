// Copyright 2024-
// Author: Ayvar Aleksiev

#include "SQLiteDBConnection.h"
#include <sstream>

namespace Notes {

bool SQLiteDBConnection::executeQuery(std::string_view query) {
   char* errMsg;
   int resultCode = sqlite3_exec(_dbPtr.get(), query.data(), nullptr, nullptr, &errMsg);
   if (resultCode != SQLITE_OK) {
      _logger->error("SQL error: '{}'", errMsg);
      sqlite3_free(errMsg);
      return false;
   }
   return true;
}

bool SQLiteDBConnection::executeParamQuery(std::string_view query, std::vector<std::string> const& queryParams)
{
   // SQL injection prevention by using parametrized queries.
   sqlite3_stmt* stmt;
   int resultCode = sqlite3_prepare_v2(_dbPtr.get(), query.data(), -1, &stmt, nullptr);
   if (resultCode != SQLITE_OK) {
      _logger->error("SQL prepare error: '{}'", sqlite3_errmsg(_dbPtr.get()));
      return false;
   }

   for (size_t i = 0; i < queryParams.size(); ++i) {
      sqlite3_bind_text(stmt, static_cast<int>(i) + 1, queryParams[i].c_str(), -1, SQLITE_STATIC);
   }

   resultCode = sqlite3_step(stmt);
   if (resultCode != SQLITE_DONE) {
      _logger->error("SQL step error: '{}'", sqlite3_errmsg(_dbPtr.get()));
      sqlite3_finalize(stmt);
      return false;
   }

   sqlite3_finalize(stmt);
   return true;
}

bool SQLiteDBConnection::beginTransaction() {
   return executeQuery("BEGIN TRANSACTION;");
}

bool SQLiteDBConnection::commitTransaction() {
   // end transaction
   return executeQuery("COMMIT;");
}

bool SQLiteDBConnection::rollbackTransaction() {
   return executeQuery("ROLLBACK;");
}

std::string
SQLiteDBConnection::sanitizeInput(const std::string& input)
{
   std::string sanitized;
   for (char c : input) {
      if (c == '\'') {
         sanitized += "''";
      }
      else {
         sanitized += c;
      }
   }
   return sanitized;
}

bool
SQLiteDBConnection::initializeDatabase()
{
   std::unique_lock<std::shared_mutex> userLock(_userTableMutex, std::defer_lock);
   std::unique_lock<std::shared_mutex> noteLock(_noteTableMutex, std::defer_lock);
   // Acquires both locks without risk of deadlock
   std::lock(userLock, noteLock);

   auto openDbFunc = [&]() {
      sqlite3* db;
      int resultCode = sqlite3_open(_dbName.c_str(), &db);
      _dbPtr.reset(db);
      if (resultCode) {
         _logger->error("Can't open database: '{}'", sqlite3_errmsg(_dbPtr.get()));
         return false;
      }
      _logger->info("'{}' database opened successfully.", _dbName);      
      return true;
   };
   if (!openDbFunc()) {
      return false;
   }

   std::string const createUsersTable = R"(
    CREATE TABLE IF NOT EXISTS Users (
        id INTEGER PRIMARY KEY AUTOINCREMENT,
        username TEXT NOT NULL UNIQUE,
        password_hash TEXT NOT NULL
    );
    )";

   std::string const createNotesTable = R"(
    CREATE TABLE IF NOT EXISTS Notes (
        id INTEGER PRIMARY KEY AUTOINCREMENT,
        user_id INTEGER,
        uid INTEGER NOT NULL,
        title TEXT NOT NULL,
        text TEXT,
        color TEXT CHECK(color IN ('yellow', 'green', 'red')),
        FOREIGN KEY (user_id) REFERENCES Users(id),
        UNIQUE(user_id, uid)
    );
    )";
   // Create individual index on username for efficient lookups.
   std::string const createUserIndex("CREATE INDEX IF NOT EXISTS idx_user_username ON Users(username);");
   // Create individual index on user_id for efficient lookups.
   std::string const createNoteUserIdIndex("CREATE INDEX IF NOT EXISTS idx_note_user_id ON Notes(user_id);");
   // Create individual index on uid for efficient lookups.
   std::string const createNoteUIDIndex("CREATE INDEX IF NOT EXISTS idx_note_uid ON Notes(uid);");
   // Create individual index on (user_id, uid) combination for efficient lookups.
   std::string const createNoteIndex("CREATE INDEX IF NOT EXISTS idx_note_user_id_uid ON Notes(user_id, uid);");

   if (!beginTransaction()) {
      return false;
   }

   bool result =
      executeQuery(createUsersTable) &&
      executeQuery(createNotesTable) &&
      executeQuery(createUserIndex) && 
      executeQuery(createNoteUserIdIndex) &&
      executeQuery(createNoteUIDIndex) &&
      executeQuery(createNoteIndex);

   if (result) {
      commitTransaction();
   }
   else {
      rollbackTransaction();
   }

   return result;
}

bool
SQLiteDBConnection::clearDatabase()
{
   std::unique_lock<std::shared_mutex> userLock(_userTableMutex, std::defer_lock);
   std::unique_lock<std::shared_mutex> noteLock(_noteTableMutex, std::defer_lock);
   // Acquires both locks without risk of deadlock
   std::lock(userLock, noteLock);

   if (!beginTransaction()) {
      return false;
   }
   std::string clearUsersTable("DELETE FROM Users;");
   std::string clearNotesTable("DELETE FROM Notes;");
   bool result = executeQuery(clearUsersTable) && executeQuery(clearNotesTable);

   if (result) {
      commitTransaction();
   }
   else {
      rollbackTransaction();
   }

   return result;
}

bool
SQLiteDBConnection::createUser(std::string const& username, std::string const& password_hash)
{
   static constexpr std::string_view query("INSERT INTO Users (username, password_hash) VALUES (?, ?);");
   std::vector<std::string> params = { sanitizeInput(username) , sanitizeInput(password_hash) };
   std::scoped_lock writeLock(_userTableMutex);
   return executeParamQuery(query, params);
}

bool
SQLiteDBConnection::deleteUser(std::string const& username, std::string const& password_hash)
{
   auto const sanitizedUsername = sanitizeInput(username);
   auto userOpt = getUserInfo(sanitizedUsername);
   if (userOpt) {
      User user = std::get<0>(userOpt.value());
      if (user.comparePasswordHash(sanitizeInput(password_hash))) {
         static constexpr std::string_view query = "DELETE FROM Users WHERE username = ?;";
         std::vector<std::string> params = { sanitizedUsername };
         std::scoped_lock writeLock(_userTableMutex);
         return executeParamQuery(query, params);
      }
   }
   return false;
}

bool
SQLiteDBConnection::changeUserPassword(
   std::string const& username,
   std::string const& oldPassword,
   std::string const& newPassword)
{
   auto const sanitezedUsername = sanitizeInput(username);
   auto userOpt = getUserInfo(sanitezedUsername);
   if (userOpt) {
      User user = std::get<0>(userOpt.value());
      if (user.comparePasswordHash(sanitizeInput(oldPassword))) {
         static constexpr std::string_view query("UPDATE Users SET password_hash = ? WHERE username = ?;");
         std::vector<std::string> params = { sanitizeInput(newPassword), sanitezedUsername };
         std::scoped_lock writeLock(_userTableMutex);
         return executeParamQuery(query, params);
      }
   }
   return false;
}

std::optional<std::tuple<User, int>>
SQLiteDBConnection::getUserInfo(const std::string& username)
{
   static constexpr std::string_view query("SELECT id, password_hash FROM Users WHERE username = ?;");
   sqlite3_stmt* stmt;
   std::shared_lock<std::shared_mutex> readLock(_userTableMutex);
   if (sqlite3_prepare_v2(_dbPtr.get(), query.data(), -1, &stmt, nullptr) == SQLITE_OK) {
      sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
      if (sqlite3_step(stmt) == SQLITE_ROW) {
         int user_id = sqlite3_column_int(stmt, 0);
         std::string passwordHash = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
         std::optional<std::tuple<User, int>> userInfo(std::make_tuple(User(username, std::move(passwordHash)), user_id));
         sqlite3_finalize(stmt);
         return userInfo;
      }
   }
   sqlite3_finalize(stmt);
   return std::nullopt;
}

bool
SQLiteDBConnection::addNotes(int user_id, std::vector<std::shared_ptr<Note>> notes)
{
   std::scoped_lock writeLock(_noteTableMutex);

   if (!beginTransaction() || notes.empty()) {
      return false;
   }
   static constexpr std::string_view query(
      "INSERT INTO Notes(user_id, uid, title, text, color) VALUES(?, ?, ?, ?, ?);");
   for (const auto& note : notes) {
      std::vector<std::string> params = {
         std::to_string(user_id),
         std::to_string(note->getUID()),
         sanitizeInput(note->getTitle()),
         sanitizeInput(note->getText()),
         colorToString(note->getColor())
      };

      if (!executeParamQuery(query, params)) {
         rollbackTransaction();
         return false;
      }
   }
   return commitTransaction();

}

std::vector<std::string>
SQLiteDBConnection::getUserNoteUIDs(int user_id)
{
   std::vector<std::string> uids;
   static constexpr std::string_view query("SELECT uid FROM Notes WHERE user_id = ?;");
   sqlite3_stmt* stmt;
   std::shared_lock<std::shared_mutex> readLock(_noteTableMutex);

   int resultCode = sqlite3_prepare_v2(_dbPtr.get(), query.data(), -1, &stmt, nullptr);
   if (resultCode != SQLITE_OK) {
      _logger->error("SQL prepare error: '{}'", sqlite3_errmsg(_dbPtr.get()));
      return uids;
   }

   sqlite3_bind_int(stmt, 1, user_id);
   while (sqlite3_step(stmt) == SQLITE_ROW) {
      uids.push_back(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)));
   }

   sqlite3_finalize(stmt);
   return uids;
}

std::vector<std::shared_ptr<Note>>
SQLiteDBConnection::getUserNotes(int user_id)
{
   std::vector<std::shared_ptr<Note>> notesResult;
   static constexpr std::string_view query("SELECT uid, title, text, color FROM Notes WHERE user_id = ?;");
   sqlite3_stmt* stmt;
   std::shared_lock<std::shared_mutex> readLock(_noteTableMutex);
   if (sqlite3_prepare_v2(_dbPtr.get(), query.data(), -1, &stmt, nullptr) == SQLITE_OK) {
      sqlite3_bind_int(stmt, 1, user_id);
      while (sqlite3_step(stmt) == SQLITE_ROW) {
         std::shared_ptr<Note> note = Note::createInstance();
         note->setUID(sqlite3_column_int(stmt, 0));
         note->setTitle(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));
         note->setText(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)));
         note->setColor(toColor(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3))));
         notesResult.push_back(std::move(note));
      }
   }
   sqlite3_finalize(stmt);
   return notesResult;
}

std::vector<std::shared_ptr<Note>>
SQLiteDBConnection::getUserNotes(int user_id, std::string const& title, std::string const& text, Color noteColor) {
   std::vector<std::shared_ptr<Note>> notesResult;
   std::ostringstream query;
   query << "SELECT uid, title, text, color FROM Notes WHERE user_id = ?";
   std::vector<std::string> params = { std::to_string(user_id) };
   if (!title.empty()) {
      query << " AND title = ?";
      params.push_back(sanitizeInput(title));
   }
   if (!text.empty()) {
      query << " AND text = ?";
      params.push_back(sanitizeInput(text));
   }
   if (noteColor != Color::invalid) {
      query << " AND color = ?";
      params.push_back(colorToString(noteColor));
   }

   sqlite3_stmt* stmt;
   std::shared_lock<std::shared_mutex> readLock(_noteTableMutex);
   if (sqlite3_prepare_v2(_dbPtr.get(), query.str().c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
      for (size_t i = 0; i < params.size(); ++i) {
         sqlite3_bind_text(stmt, static_cast<int>(i) + 1, params[i].c_str(), -1, SQLITE_STATIC);
      }
      while (sqlite3_step(stmt) == SQLITE_ROW) {
         std::shared_ptr<Note> note = Note::createInstance();
         note->setUID(sqlite3_column_int(stmt, 0));
         note->setTitle(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));
         note->setText(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)));
         note->setColor(toColor(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3))));
         notesResult.push_back(std::move(note));
      }
   }
   sqlite3_finalize(stmt);

   return notesResult;
}

std::shared_ptr<Note>
SQLiteDBConnection::getUserNoteByUID(int user_id, int uid)
{
   std::shared_ptr<Note> note = nullptr;
   static constexpr std::string_view query("SELECT uid, title, text, color FROM Notes WHERE uid = ? AND user_id = ?;");
   sqlite3_stmt* stmt;
   std::shared_lock<std::shared_mutex> readLock(_noteTableMutex);

   if (sqlite3_prepare_v2(_dbPtr.get(), query.data(), -1, &stmt, nullptr) == SQLITE_OK) {
      
      sqlite3_bind_int(stmt, 1, uid);
      sqlite3_bind_int(stmt, 2, user_id);
      
      if (sqlite3_step(stmt) == SQLITE_ROW) {
         note = Note::createInstance();
         note->setUID(sqlite3_column_int(stmt, 0));
         note->setTitle(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));
         note->setText(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)));
         note->setColor(toColor(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3))));
      }
   }
   sqlite3_finalize(stmt);
   return note;
}

bool
SQLiteDBConnection::updateNoteByUid(
   int user_id, int uid, const std::optional<std::string>& title, const std::optional<std::string>& text, const std::optional<Color>& noteColor)
{
   std::ostringstream query;
   query << "UPDATE Notes SET ";
   bool first = true;
   std::vector<std::string> params;

   if (title.has_value()) {
      if (!first) {
         query << ", ";
      }
      query << "title = ?";
      params.push_back(sanitizeInput(*title));
      first = false;
   }
   if (text.has_value()) {
      if (!first) {
         query << ", ";
      }
      query << "text = ?";
      params.push_back(sanitizeInput(*text));
      first = false;
   }
   if (noteColor.has_value()) {
      std::string colorStr = colorToString(*noteColor);
      if (!first) {
         query << ", ";
      }
      query << "color = ?";
      params.push_back(colorStr);
   }
   query << " WHERE uid = ? AND user_id = ?";
   params.push_back(std::to_string(uid));
   params.push_back(std::to_string(user_id));

   std::scoped_lock writeLock(_noteTableMutex);
   return executeParamQuery(query.str(), params);;
}

bool
SQLiteDBConnection::deleteNoteByUid(int user_id, int uid)
{
   static constexpr std::string_view query("DELETE FROM Notes WHERE uid = ? AND user_id = ?;");
   std::vector<std::string> params = { std::to_string(uid), std::to_string(user_id) };
   std::scoped_lock writeLock(_noteTableMutex);
   return executeParamQuery(query, params);
}

} // namespace Notes

