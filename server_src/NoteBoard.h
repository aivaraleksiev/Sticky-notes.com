// Copyright 2021
// Author: Ayvar Aleksiev

#pragma once


#include "Note.h"
#include "TagManager.h"
#include <string>
#include <unordered_map>
#include <vector>


namespace Notes {


class NoteBoard
{
public:
   // TODO add mutex logic

   NoteBoard() = default;

   ~NoteBoard() = default;

   NoteBoard(NoteBoard const&) = delete;

   NoteBoard& operator=(NoteBoard const&) = delete;

   void addNotes(std::vector<Note> const& notes);

   void updateNotes(std::unordered_map<size_t, Note> const& notes);

   std::unordered_map<size_t, Note>
      getNotes() const;

   // TODO must update tags info TagManager::updateTags when note is deleted if necessary.
   bool deleteNote(size_t UID);

   Note searchByTitle(std::string titleName) const; // todo return Note const& how?

   Note searchByText(std::string text) const; // todo return Note const& how?

   Note searchByTag(std::string tagName) const;

private:

   // [note uid -> notes ]
   std::unordered_map<size_t, Note> _notes;

   TagManager _tagManager;

   /// Mutex
   mutable std::shared_mutex  _mutex;
};

} // namespace Notes