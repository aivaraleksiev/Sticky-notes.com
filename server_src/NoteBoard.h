// Copyright 2021
// Author: Ayvar Aleksiev

#pragma once


#include "Note.h"
#include <string>
#include <unordered_map>


namespace Notes {


class NoteBoard
{
public:
   // TODO add mutex logic

   // TODO Add function methods like setNotes or updateNotes.
   NoteBoard() = default;

   ~NoteBoard() = default;

   NoteBoard(NoteBoard const&) = delete;

   NoteBoard& operator=(NoteBoard const&) = delete;

   void addNotes(std::vector<Note> const& notes);

   std::unordered_map<size_t, Note>
   getNotes() const;

   bool deleteNote(size_t UID);

   void addTags(std::set<std::string> tags);

   void getTags() const;

   void deleteTag(size_t UID);

   Note searchByTitle(std::string titleName) const; // todo return Note const& how?
   
   Note searchByText(std::string text) const; // todo return Note const& how?
   
   Note searchByTag(std::string tagName) const;

private:

   // [uid -> notes ]
   std::unordered_map<size_t, Note> _notes;
   
   // TODO[ uid -> ]
   // TODO std::unordered_map<size_t, Tag> _tags;

   /// Mutex
   mutable std::shared_mutex  _mutex;
};

} // namespace Notes