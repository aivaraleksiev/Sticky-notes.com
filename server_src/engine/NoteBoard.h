// Copyright 2021
// Author: Ayvar Aleksiev

#pragma once


#include "Note.h"
#include "../utils/Utils.h"
#include <optional>
#include <string>
#include <unordered_map>

namespace Notes {

using Utils::UID;

// helper structure mainly used for update note operation.
struct NoteContext
{
   UID _id;
   std::optional<std::string> _title;
   std::optional<std::string> _text;
   std::optional<Color> _noteColor;
};

class NoteBoard
{
public:
   
   // Constructor.
   NoteBoard() {}

   // Destructor
   ~NoteBoard() = default;
   
   // Copy constructor.
   NoteBoard(NoteBoard const&);

   // Assignment operator
   NoteBoard& operator=(NoteBoard const&);
   
   // Move constructor
   NoteBoard(NoteBoard&& other) noexcept;

   // Move assignment operator
   NoteBoard& operator=(NoteBoard&& other) noexcept;

   UID createNote(Note& note);

   void updateNote(NoteContext const& note);

   std::unordered_map<UID, Note> getNotes() const;

   Note getNote(UID id) const;

   bool deleteNote(UID id);

   std::vector<Note> searchByTitle(std::string titleName) const;

   std::vector<Note> searchByText(std::string text) const;

   std::vector<Note> searchByColor(Color color) const;

private:

   // [note uid -> note]
   std::unordered_map<UID, Note> _notes;

   /// Mutex
   mutable std::shared_mutex  _mutex;
};

} // namespace Notes