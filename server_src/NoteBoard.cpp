// Copyright 2021
// Author: Ayvar Aleksiev

#include "NoteBoard.h"
#include <stdexcept>

namespace Notes {

void
NoteBoard::createNote(Note& note)
{
   std::scoped_lock writeLock(_mutex);
   if (note.getUID() == INVALID_UID) {
      auto id = Utils::UIDGenerator::generateUID();
      note.setUID(id);
   }
   _notes.emplace(note.getUID(), note);
}

void
NoteBoard::updateNote(NoteContext const& note)
{ // todo think about this method how it will be read from json. do we need std::optional vars.
   std::scoped_lock writeLock(_mutex);
   auto it = _notes.find(note.id);
   if (it != _notes.end()) {
      auto& editNote = it->second;
      
      if (note._title) {
         editNote.setTitle(*note._title);
      }
      if (note._text) {
         editNote.setText(*note._text);
      }
      if (note._noteColor) {
         editNote.setColor(*note._noteColor);
      }
   }
   // todo add exception logic
}

std::unordered_map<UID, Note>
NoteBoard::getNotes() const
{
   std::shared_lock<std::shared_mutex> readLock(_mutex);
   return _notes;
}

Note
NoteBoard::getNote(UID id) const
{
   std::shared_lock<std::shared_mutex> readLock(_mutex);
   auto noteIt = _notes.find(id);
   if (noteIt != _notes.end()) {
      return noteIt->second;
   }
   return Note();
   // todo add excetion or return empty note with invalid uid.
}

bool
NoteBoard::deleteNote(UID id)
{
   std::scoped_lock writeLock(_mutex);
   return _notes.erase(id);
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
NoteBoard::searchByColor(Note::Color color) const
{
   std::shared_lock<std::shared_mutex> readLock(_mutex);
   auto result = std::find_if(
      _notes.begin(),
      _notes.end(),
      [&color](auto const& val) {
         auto const& note = val.second;
         return note.getColor() == color;
      });
   if (result != _notes.end()) {
      return result->second;
   }
   // TODO throw exception. This may throw warning/error. Not all return paths are covered.
   return Note();;
}

} // namespace Notes