// Copyright 2021
// Author: Ayvar Aleksiev

#include "AuthenticationManager.h"


namespace Notes {

AuthenticateionManager*
AuthenticateionManager::getInstance()
{
   static std::unique_ptr<AuthenticateionManager> sManagerPtr(
      std::make_unique<AuthenticateionManager>());
   return sManagerPtr.get();
}

} //namspace Notes
