// Copyright 2021
// Author: Ayvar Aleksiev

#pragma once


#include "Note.h"
#include "Utils.h"
#include <boost/optional.hpp>
#include <string>
#include <unordered_map>
#include <vector>

namespace Notes {

using Utils::UID;

// helper structure mainly used for update note operation.
struct NoteContext
{
   UID id;
   boost::optional<std::string> _title;
   boost::optional<std::string> _text;
   boost::optional<Note::Color> _noteColor;
};

class NoteBoard
{
public:
   // TODO add mutex logic

   NoteBoard() = default;
   ~NoteBoard() = default;
   NoteBoard(NoteBoard const&) = delete;
   NoteBoard& operator=(NoteBoard const&) = delete;

   void createNote(Note const& note);

   void updateNote(NoteContext const& note);

   std::unordered_map<size_t, Note>
      getNotes() const;

   bool deleteNote(UID id);

   Note searchByTitle(std::string titleName) const; // todo return Note const& how?

   Note searchByText(std::string text) const;

   Note searchByColor(Note::Color color) const;

private:

   // [note uid -> notes ]
   std::unordered_map<UID, Note> _notes;

   /// Mutex
   mutable std::shared_mutex  _mutex;
};

} // namespace Notes