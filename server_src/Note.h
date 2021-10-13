// Copyright 2021
// Author: Ayvar Aleksiev

#pragma once

#include "Utils.h"

#include <nlohmann/json.hpp>
#include <shared_mutex>
#include <string>

namespace Notes {

using Utils::UID;


// TODO ADD addtional checks and throw exceptions.
// Synchronized class describing a note.
// Every note consists of: Title, Text, and note Color.
class Note
{
public:
   enum class Color {
      yellow = 0,
      green = 1,
      red = 2
   };
   // map Color values to JSON as strings
   NLOHMANN_JSON_SERIALIZE_ENUM(Color, {
       {Color::yellow, "yellow"},
       {Color::green, "green"},
       {Color::red, "red"}
      })
   NLOHMANN_DEFINE_TYPE_INTRUSIVE(Note, _id, _title, _text, _noteColor)
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

   void setUID(UID newId) {
      _id = newId;
   }

   UID getUID() const {
      return _id;
   }

   std::string const& getTitle() const;
   
   std::string const& getText() const;  

   Note::Color getColor() const;

   void setTitle(std::string newTitle);

   void setText(std::string newText);    

   void setColor(Note::Color noteColor);

private:

   UID _id{0};

   std::string _title;
   
   std::string _text;

   Note::Color _noteColor { Note::Color::yellow };

   /// Mutex
   mutable std::shared_mutex  _mutex;
};

} // namespace Notes