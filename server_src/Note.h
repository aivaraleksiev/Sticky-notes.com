#pragma once
// sticky note temporary class

#include <set>
#include <string>

class Note
{
public:
   using Tag = std::string;
   using Title = std::string;
   using Text = std::string;

   Title getTitle() const
   {
      return _title;
   }

   std::string getText() const
   {
      return _title;
   }

   std::set<Tag> getTags() const
   {
      return _tags;
   }

   void editTitle(Title title)
   {
      _title = title;
   }

   void editText(Text text)
   {
      _text = text;
   }

   void addTitle(Title newTitle)
   {
      _title = newTitle;
   }

   void addTag(Tag newTag)
   {
      _tags.insert(newTag);
   }

   void removeTag(Tag tag)
   {
      _tags.erase(tag);
   }

private:
   Title _title;
   Text _text;
   std::set<Tag> _tags;
};