#pragma once

#include "Note.h"
#include <string>
#include <map>
#include <vector>
#include <stdexcept>

class NoteBoard
{
public:

   void addNote(Note const& note)
   {
      _noteBoard.push_back(note);
   }

   std::vector<Note> getNotes()
   {
      return _noteBoard;
   }

   // not implemented
   bool deleteNote(Note const& note)
   {
      throw std::logic_error("Function not yet implemented");
      return false;
   }

   // not implemented
   Note searchByTitle(Note::Title titleName)
   {
      throw std::logic_error("Function not yet implemented");
      return Note();
   }

   // not implemented
   Note searchByText(Note::Text text)
   {
      throw std::logic_error("Function not yet implemented");
      return Note();
   }

   // not implemented
   Note searchByTag(Note::Tag tagName)
   {
      throw std::logic_error("Function not yet implemented");
      return Note();
   }
private:
   std::vector<Note> _noteBoard;
};