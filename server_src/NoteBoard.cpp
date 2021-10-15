// Copyright 2021
// Author: Ayvar Aleksiev

#include "NoteBoard.h"
#include <stdexcept>

namespace Notes {

UID
NoteBoard::createNote(Note& note)
{
   std::scoped_lock writeLock(_mutex);
   if (note.getUID() == INVALID_UID) {
      auto id = Utils::UIDGenerator::generateUID();
      note.setUID(id);
   }
   _notes.emplace(note.getUID(), note);
   return note.getUID();
}

void
NoteBoard::updateNote(NoteContext const& note)
{ // todo think about this method how it will be read from json. do we need std::optional vars.
   std::scoped_lock writeLock(_mutex);
   auto it = _notes.find(note._id);
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

std::vector<Note>
NoteBoard::searchByTitle(std::string titleName) const
{
   std::shared_lock<std::shared_mutex> readLock(_mutex);
   std::vector<Note> result;
   if (titleName == "") {
      return result;
   }
   for (auto const& [key, note] : _notes) {
      if (note.getTitle() == titleName) {
         result.push_back(note);
      }
   }
   return result;
}

std::vector<Note>
NoteBoard::searchByText(std::string text) const
{
   std::shared_lock<std::shared_mutex> readLock(_mutex);
   std::vector<Note> result;
   for (auto const& [key, note] : _notes) {
      if (note.getText() == text) {
         result.push_back(note);
      }
   }
   return result;
}

std::vector<Note>
NoteBoard::searchByColor(Color color) const
{
   std::shared_lock<std::shared_mutex> readLock(_mutex);
   // TODO use ranges, once the compiler supports them.
   std::vector<Note> result;
   if (color == Color::invalid) {
      return result;
   }
   for (auto const& [key, value] : _notes) {
      if (value.getColor() == color) {
         result.push_back(value);
      }
   }
   return result;
}

} // namespace Notes