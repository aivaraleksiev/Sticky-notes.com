// Copyright 2021-2022
// Author: Ayvar Aleksiev

#include "Note.h"
#include <mutex>

namespace Notes {

Color toColor(std::string const& val)
{
   if (val == "yellow") {
      return Color::yellow;
   }
   else if (val == "green") {
      return Color::green;
   }
   else if (val == "red") {
      return Color::red;
   }
   return Color::invalid;
}

std::string const& colorToString(Color color) {
   switch (color) {
   case Color::red:
      return std::string("red");
      break;
   case Color::green:
      return std::string("green");
      break;
   case Color::yellow:
      return std::string("yellow");
      break;
   case Color::invalid:
      return std::string("invalid");
      break;
   default:
      return std::string("invalid");
   }
}

std::string const&
Note::getTitle() const
{
   return _title;
}

std::string const&
Note::getText() const
{
   return _text;
}

Color
Note::getColor() const
{
   return _noteColor;
}

void
Note::setTitle(std::string newTitle)
{
   _title = newTitle;
}

void
Note::setText(std::string newText)
{
   _text = newText;
}

void
Note::setColor(Color noteColor)
{
   _noteColor = noteColor;
}

}  // namespace N
