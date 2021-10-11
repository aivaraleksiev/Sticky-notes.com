// Copyright 2021
// Author: Ayvar Aleksiev

#pragma once

#include <set>
#include <shared_mutex>
#include <string>
#include <unordered_map>
#include <unordered_set>

namespace Notes {

//helper structure
struct Tag
{
   std::string _name;
   // Contains the noteIds to which this tag is attached.
   std::unordered_set<size_t> _tagAssociations;
};

class TagManager
{
public:

   TagManager() = default;

   ~TagManager() = default;

   TagManager(TagManager const&) = delete;

   TagManager& operator=(TagManager const&) = delete;

   void addTags(std::vector<Tag> tags);

   void updateTags(std::set<std::string> tags);

   std::unordered_map< size_t, Tag> getTags() const;
   
   // TODO must update tags info NoteBoard::updateNotes when note is deleted if necessary.
   bool deleteTag(size_t UID);


private:

   // [tag uid -> Tag]
   std::unordered_map< size_t, Tag> _tagInfo;

   /// Mutex
   mutable std::shared_mutex  _mutex;
};

} // namespace Notes