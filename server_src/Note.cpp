// Copyright 2021
// Author: Ayvar Aleksiev

#include "Note.h"
#include <mutex>

namespace Notes {

Color toColor(std::string val)
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
   std::shared_lock<std::shared_mutex> readLock(other._mutex);
   _title = other._title;
   _text = other._text;
   _noteColor = other._noteColor;
   _id = other._id;
}

Note&
Note::operator=(Note const& other)
{
   std::unique_lock<std::shared_mutex> writeLock(_mutex, std::defer_lock);
   std::shared_lock<std::shared_mutex> readLock(other._mutex, std::defer_lock);
   std::lock(writeLock, readLock);
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
   std::shared_lock<std::shared_mutex> readLock(other._mutex);
   _title = std::move(other._title);
   _text = std::move(other._text);
   _noteColor = other._noteColor;
   _id = other._id;
}

Note&
Note::operator=(Note&& other) noexcept
{
   std::unique_lock<std::shared_mutex> writeLock(_mutex, std::defer_lock);
   std::shared_lock<std::shared_mutex> readLock(other._mutex, std::defer_lock);
   std::lock(writeLock, readLock);

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
   std::shared_lock<std::shared_mutex> readLock(_mutex);
   return _title;
}

std::string const&
Note::getText() const
{
   std::shared_lock<std::shared_mutex> readLock(_mutex);
   return _text;
}

Color
Note::getColor() const
{
   std::shared_lock<std::shared_mutex> readLock(_mutex);
   return _noteColor;
}

void
Note::setTitle(std::string newTitle)
{
   std::scoped_lock writeLock(_mutex);
   _title = newTitle;
}

void
Note::setText(std::string newText)
{
   std::scoped_lock writeLock(_mutex);
   _text = newText;
}

void
Note::setColor(Color noteColor)
{
   std::scoped_lock writeLock(_mutex);
   _noteColor = noteColor;
}

}  // namespace N