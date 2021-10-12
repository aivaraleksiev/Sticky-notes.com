// Copyright 2021
// Author: Ayvar Aleksiev

#pragma once

#include "Utils.h"
#include <unordered_set>
#include <string>
#include <shared_mutex>

namespace Notes {

using Utils::UID;

// TODO ADD addtional checks and throw exceptions.

// Synchronized class describing a note.
// Every note consists of: Title, Text, and note Color.
class Note
{
public:
   
   enum Color {
      yellow = 0,
      green,
      red
   };

   // Constructor
   Note() = default;

   // Destructor
   ~Note() = default;

   // Copy constructor.
   Note(Note const& other);

   // Assignment operator
   Note& operator=(Note const& other);

   // Move constructor
   Note(Note&& other) noexcept;

   // Move assignment operator
   Note& operator=(Note&& other) noexcept;

   std::string const& getTitle() const;
   
   std::string const& getText() const;  

   Note::Color getColor() const;

   void setTitle(std::string newTitle);

   void setText(std::string newText);    

   void setColor(Note::Color noteColor);

private:

   std::string _title;
   
   std::string _text;

   Color _noteColor { Color::yellow };

   /// Mutex
   mutable std::shared_mutex  _mutex;
};

} // namespace Notes