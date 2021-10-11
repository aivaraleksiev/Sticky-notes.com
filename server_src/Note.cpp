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
   _attachedTags = other._attachedTags;
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
      _attachedTags = other._attachedTags;
   }
   return *this;
}

Note::Note(Note&& other) noexcept
{
   std::shared_lock<std::shared_mutex> readLock(other._mutex);
   _title = std::move(other._title);
   _text = std::move(other._text);
   _attachedTags = std::move(other._attachedTags);
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
      _attachedTags = std::move(other._attachedTags);
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

std::set<std::string>
Note::getTags() const
{
   std::shared_lock<std::shared_mutex> readLock(_mutex);
   return _attachedTags;
}

void
Note::setTitle(std::string newTitle)
{
   _mutex.lock(); //writeLock
   _title = newTitle;
   _mutex.unlock();
}

void
Note::setText(std::string newText)
{
   _mutex.lock(); //writeLock
   _text = newText;
   _mutex.unlock();
}

void
Note::setAttachedTags(std::set<std::string> newTags)
{
   _mutex.lock(); //writeLock
   _attachedTags = newTags;
   _mutex.unlock();
}

}  // namespace N