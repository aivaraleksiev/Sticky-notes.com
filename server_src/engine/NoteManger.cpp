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

std::shared_ptr<NoteBoard>
NoteManager::getUserNoteBoard(std::string const& user)
{
   std::shared_lock<std::shared_mutex> readLock(_mutex);
   auto it = _noteboards.find(user);
   if (it == _noteboards.end()) {
      throw Utils::HttpException(restinio::status_not_found(), "User not found.");
   }
   return it->second;
}

std::vector<std::shared_ptr<Note>>
NoteManager::getUserNotes(std::string username)
{
   std::vector<std::shared_ptr<Note>> result;
   std::optional<std::tuple<User, int>> userInfo = 
      _dbService->getUserInfo(username);
   if (userInfo) {
      result = _dbService->getUserNotes(std::get<1>(*userInfo));
   }
   return result;
}

} // namespac eNotes
