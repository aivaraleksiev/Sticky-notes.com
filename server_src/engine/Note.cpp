// Copyright 2021
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

Note::Note(Note const& other)
{
   _title = other._title;
   _text = other._text;
   _noteColor = other._noteColor;
   _id = other._id;
}

Note&
Note::operator=(Note const& other)
{
   if (this != &other) {
      _title = other._title;
      _text = other._text;
      _noteColor = other._noteColor;
      _id = other._id;
   }
   return *this;
}

Note::Note(Note&& other) noexcept
{
   _title = std::move(other._title);
   _text = std::move(other._text);
   _noteColor = other._noteColor;
   _id = other._id;
}

Note&
Note::operator=(Note&& other) noexcept
{
   if (this != &other) {
      _title = std::move(other._title);
      _text = std::move(other._text);
      _noteColor = other._noteColor;
      _id = other._id;
   }
   return *this;
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