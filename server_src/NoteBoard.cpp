// Copyright 2021
// Author: Ayvar Aleksiev

#include "NoteBoard.h"
#include <stdexcept>

namespace Notes {

void
NoteBoard::createNote(Note const& note)
{
   std::lock_guard<std::shared_mutex> writeLock(_mutex);
   _notes.emplace(Utils::UIDGenerator::generateUID(), note);  
}

void
NoteBoard::updateNote(NoteContext const& note)
{
   std::lock_guard<std::shared_mutex> writeLock(_mutex);
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

bool
NoteBoard::deleteNote(UID id)
{
   std::lock_guard<std::shared_mutex> writeLock(_mutex);   
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