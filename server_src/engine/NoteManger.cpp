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

void
NoteManager::addUserNoteBoard(std::string const& user)
{
   std::scoped_lock writeLock(_mutex);
   _noteboards.emplace(user, std::make_shared<NoteBoard>());
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

} // namespac eNotes