// Copyright 2021-2022
// Author: Ayvar Aleksiev

#include "NoteBoard.h"
#include "../utils/HttpException.h"

#include <ranges>


namespace Notes {

UID
NoteBoard::createNote(std::shared_ptr<Note> notePtr)
{
   std::scoped_lock writeLock(_mutex);
   if (notePtr->getUID() == INVALID_UID) {
      notePtr->setUID(
         Utils::generateUID());
   }
   _notes->emplace(notePtr->getUID(), notePtr);
   return notePtr->getUID();
}

void
NoteBoard::updateNote(NoteContext const& note)
{
   std::scoped_lock writeLock(_mutex);
   auto it = _notes->find(note._id);
   if (it == _notes->end()) {
      std::ostringstream reason;
      reason << "Note id '" << note._id << "' not found.";
      throw Utils::HttpException(restinio::status_not_found(), reason.str());
   }   
   std::shared_ptr<Note>& updateNote = it->second;
   
   if (note._title) {
      updateNote->setTitle(*note._title);
   }
   if (note._text) {
      updateNote->setText(*note._text);
   }
   if (note._noteColor && (note._noteColor != Color::invalid)) {
      updateNote->setColor(*note._noteColor);
   }
   else {
       std::stringstream errReason;
       errReason << "Invalid color for NoteId '" << note._id << "'";
       throw Utils::HttpException(restinio::status_bad_request(), errReason.str());
   }
}

std::shared_ptr<std::unordered_map<UID, std::shared_ptr<Note>>>
NoteBoard::getNotes() const
{
   std::shared_lock<std::shared_mutex> readLock(_mutex);
   return _notes;
}

std::shared_ptr<Note>
NoteBoard::getNote(UID id) const
{
   std::shared_lock<std::shared_mutex> readLock(_mutex);
   auto noteIt = _notes->find(id);
   if (noteIt == _notes->end()) {
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
   return _notes->erase(id);
}

std::vector<std::shared_ptr<Note>>
NoteBoard::searchByTitle(std::string titleName) const
{
   std::shared_lock<std::shared_mutex> readLock(_mutex);
   if (titleName.empty()) {
      throw Utils::HttpException(restinio::status_bad_request(), "Invalid empty title.");
   }

   std::vector<std::shared_ptr<Note>> result;
   for (auto const& [uid, notePtr] : *_notes) {
      if (notePtr->getTitle() == titleName) {
         result.push_back(notePtr);
      }
   }

   return result;
}

std::vector<std::shared_ptr<Note>>
NoteBoard::searchByText(std::string text) const
{
   std::shared_lock<std::shared_mutex> readLock(_mutex);

   std::vector<std::shared_ptr<Note>> result;
   for (auto const& [uid, notePtr] : *_notes) {
      if (notePtr->getText() == text) {
         result.push_back(notePtr);
      }
   }

   return result;
}

std::vector<std::shared_ptr<Note>>
NoteBoard::searchByColor(Color color) const
{
   std::shared_lock<std::shared_mutex> readLock(_mutex);
   if (color == Color::invalid) {
      throw Utils::HttpException(restinio::status_bad_request(), "Invalid color.");
   }
   
   std::vector<std::shared_ptr<Note>> result;
   for (auto const& [uid, notePtr] : *_notes) {
      if (notePtr->getColor() == color) {
         result.push_back(notePtr);
      }
   }

   return result;
}

} // namespace Notes