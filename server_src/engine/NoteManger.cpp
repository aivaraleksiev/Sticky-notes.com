// Copyright 2021
// Author: Ayvar Aleksiev

#include "NoteManager.h"

namespace Notes {

NoteManager*
NoteManager::getInstance()
{
   static std::unique_ptr<NoteManager> sManagerPtr(
      std::make_unique<NoteManager>());
   return sManagerPtr.get();
}

} // namespac eNotes