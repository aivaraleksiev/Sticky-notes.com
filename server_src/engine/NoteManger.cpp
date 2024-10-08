// Copyright 2021
// Author: Ayvar Aleksiev

#include "NoteManager.h"

namespace Notes {

NoteManager*
NoteManager::getInstance()
{
   static std::unique_ptr<NoteManager> sManagerPtr(
      std::make_unique<NoteManager>());
   return sManagerPtr.get();
}

std::optional<int> NoteManager::getUserID(std::string const& username)
{
   std::optional<int> result;
   std::optional<std::tuple<User, int>> userInfo =
      _dbService->getUserInfo(username);
   if (userInfo) {
      result = std::get<1>(*userInfo);
   }
   return result;
}

std::vector<std::shared_ptr<Note>>
NoteManager::getUserNotes(std::string const& username)
{
   std::vector<std::shared_ptr<Note>> result;
   std::optional<int> userId = getUserID(username);
   if (userId) {
      result = _dbService->getUserNotes(std::get<1>(*userId));
   }
   return result;
}

std::vector<std::shared_ptr<Note>>
NoteManager::getUserNotes(
   std::string const& username,
   std::string const& title,
   std::string const& text,
   Color noteColor)
{
   std::vector<std::shared_ptr<Note>> result;
   std::optional<int> userId = getUserID(username);
   if (userId) {
      result = _dbService->getUserNotes(*userId, title, text, noteColor);
   }
   return result;
}

std::shared_ptr<Note> 
NoteManager::getUserNoteByUID(std::string const& username, UID uid)
{
   std::shared_ptr<Note> result;
   std::optional<int> userId = getUserID(username);
   if (userId) {
      result = _dbService->getUserNoteByUID(*userId, uid);
   }
   return result;
}

bool 
NoteManager::addNotes(std::string const& username, std::vector<std::shared_ptr<Note>> const& newNotes)
{
   bool result = false;
   std::optional<int> userId = getUserID(username);
   if (userId) {
      result = _dbService->addNotes(*userId, newNotes);
   }
   return result;
}

bool
NoteManager::updateNoteByUid(
   std::string const& username,
   int uid,
   const std::optional<std::string>& title,
   const std::optional<std::string>& text,
   const std::optional<Color>& noteColor)
{
   bool result = false;
   std::optional<int> userId = getUserID(username);
   if (userId) {
      result =
         _dbService->updateNoteByUid(
            *userId,
            uid,
            title, 
            text,
            noteColor);
   }
   return result;
}

bool
NoteManager::deleteNoteByUid(std::string const& username, int uid)
{
   bool result = false;
   std::optional<int> userId = getUserID(username);
   if (userId) {
      result = _dbService->deleteNoteByUid(*userId, uid);
   }
   return result;
}

bool
NoteManager::deleteNotesForUser(std::string const& username)
{
   bool result = false;
   std::optional<int> userId = getUserID(username);
   if (userId) {
      result = _dbService->deleteAllNotesByUserId(*userId);
   }
   return result;
}

} // namespace Notes
