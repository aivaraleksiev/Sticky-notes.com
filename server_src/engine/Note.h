// Copyright 2021-2022
// Author: Ayvar Aleksiev

#pragma once

#include "../utils/Utils.h"

#include <nlohmann/json.hpp>
#include <shared_mutex>
#include <string>

using nlohmann::json;
using Notes::Utils::UID;

namespace Notes {

enum class Color {
   invalid = -1,
   yellow = 0,
   green = 1,
   red = 2
};
// map Color values to JSON as strings
NLOHMANN_JSON_SERIALIZE_ENUM(Color, {
    {Color::invalid, "invalid"},
    {Color::yellow, "yellow"},
    {Color::green, "green"},
    {Color::red, "red"}
   })

Color toColor(std::string const& val);
std::string const& colorToString(Color color);

// Synchronized class describing a note.
// Every note consists of: Title, Text, and note Color.
class Note
{
public:
   
   // Constructor
   Note(UID id = Utils::generateUID())
      : _id(id)
   {}

   // Destructor
   ~Note() = default;

   // Copy constructor.
   Note(Note const& other) = delete;

   // Assignment operator
   Note& operator=(Note const& other) = delete;

   void setUID(UID newId) {
      _id = newId;
   }

   UID getUID() const {
      return _id;
   }

   std::string const& getTitle() const;
   
   std::string const& getText() const;  

   Color getColor() const;

   void setTitle(std::string newTitle);

   void setText(std::string newText);    

   void setColor(Color noteColor);

   friend void to_json(json& j, Note const& obj) {
    j = json{ {"id", obj._id}, {"title", obj._title}, {"text", obj._text}, {"color", obj._noteColor} };
   }

   friend void from_json(json const& j, Note& obj) {
      j.at("id").get_to<UID>(obj._id);
      j.at("title").get_to(obj._title);
      j.at("text").get_to(obj._text);
      j.at("color").get_to<Color>(obj._noteColor);
   }

private:
   // Constructor
   Note(UID id = Utils::generateUID())
      : _id(id)
   {}

   UID _id;

   std::string _title;
   
   std::string _text;

   Color _noteColor { Color::yellow };
};

} // namespace Notes
