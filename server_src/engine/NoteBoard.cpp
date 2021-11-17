// Copyright 2021
// Author: Ayvar Aleksiev

#include "NoteBoard.h"
#include "../utils/HttpException.h"

#include <ranges>


namespace Notes {

NoteBoard::NoteBoard(NoteBoard const& other)
{
   std::shared_lock<std::shared_mutex> readLock(other._mutex);
   _notes = other._notes;
}

NoteBoard&
NoteBoard::operator=(NoteBoard const& other)
{
   std::unique_lock<std::shared_mutex> writeLock(_mutex, std::defer_lock);
   std::shared_lock<std::shared_mutex> readLock(other._mutex, std::defer_lock);
   std::lock(writeLock, readLock);
   if (this != &other) {
      _notes = other._notes;
   }
   return *this;
}

NoteBoard::NoteBoard(NoteBoard&& other) noexcept
{
   std::shared_lock<std::shared_mutex> readLock(other._mutex);
   _notes = other._notes;
}

NoteBoard&
NoteBoard::operator=(NoteBoard&& other) noexcept
{
   std::unique_lock<std::shared_mutex> writeLock(_mutex, std::defer_lock);
   std::shared_lock<std::shared_mutex> readLock(other._mutex, std::defer_lock);
   std::lock(writeLock, readLock);

   if (this != &other) {
      _notes = other._notes;
   }
   return *this;
}

UID
NoteBoard::createNote(Note& note)
{
   std::scoped_lock writeLock(_mutex);
   if (note.getUID() == INVALID_UID) {      
      note.setUID(
         Utils::generateUID());
   }
   _notes.emplace(note.getUID(), note);
   return note.getUID();
}

void
NoteBoard::updateNote(NoteContext const& note)
{
   std::scoped_lock writeLock(_mutex);
   auto it = _notes.find(note._id);
   if (it == _notes.end()) {
      std::ostringstream reason;
      reason << "Note id '" << note._id << "' not found.";
      throw Utils::HttpException(restinio::status_not_found(), reason.str());
   }   
   auto& editNote = it->second;
   
   if (note._title) {
      editNote.setTitle(*note._title);
   }
   if (note._text) {
      editNote.setText(*note._text);
   }
   if (note._noteColor && (note._noteColor != Color::invalid)) {
      editNote.setColor(*note._noteColor);
   }
   else {
       std::stringstream errReason;
       errReason << "Invalid color for NoteId '" << note._id << "'";
       throw Utils::HttpException(restinio::status_bad_request(), errReason.str());
   }
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
   if (noteIt == _notes.end()) {
      std::ostringstream reason;
      reason << "Note with id '" << id << "'.";
      throw Utils::HttpException(restinio::status_not_found(), reason.str());
   }
   return noteIt->second;
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
   if (titleName.empty()) {
      throw Utils::HttpException(restinio::status_bad_request(), "Invalid empty title.");
   }

   std::vector<Note> result;
   auto sameTitle = [titleName](auto note) {return note.getTitle() == titleName; };
   for (auto const& note : std::views::values(_notes) | std::views::filter(sameTitle)) {
      result.push_back(note);
   }

   return result;
}

std::vector<Note>
NoteBoard::searchByText(std::string text) const
{
   std::shared_lock<std::shared_mutex> readLock(_mutex);

   std::vector<Note> result;
   auto sameText = [text](auto note) {return note.getText() == text; };
   for (auto const& note : std::views::values(_notes) | std::views::filter(sameText)) {
      result.push_back(note);
   }

   return result;
}

std::vector<Note>
NoteBoard::searchByColor(Color color) const
{
   std::shared_lock<std::shared_mutex> readLock(_mutex);
   if (color == Color::invalid) {
      throw Utils::HttpException(restinio::status_bad_request(), "Invalid color.");
   }
   
   std::vector<Note> result;
   auto sameColor = [color](auto note) {return note.getColor() == color; };
   for (auto const& note : std::views::values(_notes) | std::views::filter(sameColor)) {
      result.push_back(note);
   }

   return result;
}

} // namespace Notes