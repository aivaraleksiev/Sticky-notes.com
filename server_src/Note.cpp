// Copyright 2021
// Author: Ayvar Aleksiev

#include "Note.h"
#include <mutex>

namespace Notes {

Note::Note(Note const& other)
{
   std::shared_lock<std::shared_mutex> readLock(other._mutex);
   _title = other._title;
   _text = other._text;
   _noteColor = other._noteColor;
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
   }
   return *this;
}

Note::Note(Note&& other) noexcept
{
   std::shared_lock<std::shared_mutex> readLock(other._mutex);
   _title = std::move(other._title);
   _text = std::move(other._text);
   _noteColor = other._noteColor;
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

Note::Color
Note::getColor() const
{
   std::shared_lock<std::shared_mutex> readLock(_mutex);
   return _noteColor;
}

void
Note::setTitle(std::string newTitle)
{
   _mutex.lock(); //writeLock
   _title = newTitle;
}

void
Note::setText(std::string newText)
{
   std::lock_guard<std::shared_mutex> writeLock(_mutex);
   _text = newText;
}

void
Note::setColor(Note::Color noteColor)
{
   std::lock_guard<std::shared_mutex> writeLock(_mutex);
   _noteColor = noteColor;
}

}  // namespace N