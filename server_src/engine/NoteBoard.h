// Copyright 2021-2022
// Author: Ayvar Aleksiev

#pragma once


#include "Note.h"
#include "../utils/Utils.h"

#include <memory>
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
   NoteBoard() :
      _notes(std::make_shared<std::unordered_map<UID,std::shared_ptr<Note>>>()) {}

   // Destructor
   ~NoteBoard() = default;

   // Copy constructor.
   NoteBoard(NoteBoard const&) = delete;

   // Assignment operator
   NoteBoard& operator=(NoteBoard const&) = delete;

   UID createNote(std::shared_ptr<Note> notePtr);

   void updateNote(NoteContext const& note);

   std::shared_ptr<std::unordered_map<UID, std::shared_ptr<Note>>> getNotes() const;

   std::shared_ptr<Note> getNote(UID id) const;

   bool deleteNote(UID id);

   std::vector<std::shared_ptr<Note>> searchByTitle(std::string titleName) const;

   std::vector<std::shared_ptr<Note>> searchByText(std::string text) const;

   std::vector<std::shared_ptr<Note>> searchByColor(Color color) const;

private:

   // [note uid -> note]
   std::shared_ptr<std::unordered_map<UID, std::shared_ptr<Note>>> _notes;

   /// Mutex
   mutable std::shared_mutex _mutex;
};

} // namespace Notes