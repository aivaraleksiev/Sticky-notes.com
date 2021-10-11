// Copyright 2021
// Author: Ayvar Aleksiev

#include "TagManager.h"
#include "Utils.h"
#include <mutex>
#include <stdexcept>

namespace Notes {

void
TagManager::addTags(std::vector<Tag> tags)
{
   std::lock_guard<std::shared_mutex> writeLock(_mutex);
   for (auto const& tag : tags) {      
      _tagInfo.emplace(Utils::UIDGenerator::generateUID(), tag);
   }   
}

void
TagManager::updateTags(std::set<std::string> tags)
{
   std::lock_guard<std::shared_mutex> writeLock(_mutex);
   throw std::logic_error("TODO Function not yet implemented");
   // may be trikcy with optiona lfileds. Loo at API doc.
   // Maybe add function args for every field/value that can be updated.
}

std::unordered_map< size_t, Tag>
TagManager::getTags() const
{
   std::shared_lock<std::shared_mutex> readLock(_mutex);
   return _tagInfo;
}

bool
TagManager::deleteTag(size_t UID)
{
   std::lock_guard<std::shared_mutex> writeLock(_mutex);
   // TODO must update tags info NoteBoard::updateNotes when note is deleted if necessary.
   return _tagInfo.erase(UID);
}

} // namespace Notes