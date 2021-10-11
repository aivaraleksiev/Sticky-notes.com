// Copyright 2021
// Author: Ayvar Aleksiev

#pragma once

#include <set>
#include <string>
#include <shared_mutex>

namespace Notes {

// Synchronized class describing a note.
// Every note consists of: Title, Text tags attached to it.
class Note
{
public:
   // TODO ADD addtional checks and throw exceptions.
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

   std::set<std::string> getTags() const;
   
   void setTitle(std::string newTitle);

   void setText(std::string newText);   

   void setAttachedTags(std::set<std::string> newTags);

private:
   std::string _title;
   std::string _text;
   std::set<std::string> _attachedTags; // TODO Think about this structure

   /// Mutex
   mutable std::shared_mutex  _mutex;
};

} // namespace Notes