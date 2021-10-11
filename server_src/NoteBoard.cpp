// Copyright 2021
// Author: Ayvar Aleksiev

#include "NoteBoard.h"
#include "Utils.h"
#include <stdexcept>

namespace Notes {

void
NoteBoard::addNotes(std::vector<Note> const& notes)
{
   std::lock_guard<std::shared_mutex> writeLock(_mutex);
   // TODO Is there a std algorithm doing this.
   for (auto&& note : notes)
   {
      _notes.emplace(Utils::UIDGenerator::generateUID(), note);
   }
}

void
NoteBoard::updateNotes(std::unordered_map<size_t, Note> const& notes)
{
 // TODO IT MAY BE TRICKY WITH WHIS OPTIONAL FIELDS. Look at REST API DOC
// Can you make some params optional. Not so trivial though.
   //// Maybe add function args for every field/value that can be updated.
}

std::unordered_map<size_t, Note>
NoteBoard::getNotes() const
{
   std::shared_lock<std::shared_mutex> readLock(_mutex);
   return _notes;
}

bool
NoteBoard::deleteNote(size_t UID)
{
   std::lock_guard<std::shared_mutex> writeLock(_mutex);
   // TODO must update tags info TagManager::updateTags when note is deleted if necessary.
   return _notes.erase(UID);
}

Note
NoteBoard::searchByTitle(std::string titleName) const
{
   std::shared_lock<std::shared_mutex> readLock(_mutex);
   auto result = std::find_if(
      _notes.begin(),
      _notes.end(),
      [&titleName](auto const& val) {
         auto const& note = val.second;
         return note.getTitle() == titleName;
      });
   if (result != _notes.end()) {
      return result->second;
   }
   // TODO throw exception. This may throw warning/error. Not all return paths are covered.
   return Note();;
}

Note
NoteBoard::searchByText(std::string text) const
{
   std::shared_lock<std::shared_mutex> readLock(_mutex);
   auto result = std::find_if(
      _notes.begin(),
      _notes.end(),
      [&text](auto const& val) {
         auto const& note = val.second;
         auto findResult = note.getText().find(text);
         return findResult != std::string::npos;
      });
   if (result != _notes.end()) {
      return result->second;
   }
   // TODO throw exception. This may throw warning/error. Not all return paths are covered.
   return Note();
}

Note
NoteBoard::searchByTag(std::string tagName) const
{
   std::shared_lock<std::shared_mutex> readLock(_mutex);
   throw std::logic_error("TODO Function not yet implemented");
   return Note();
}

} // namespace Notes