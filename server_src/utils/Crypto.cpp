// Copyright 2021
// Author: Ayvar Aleksiev

#include <openssl/evp.h>
#include <sstream>
#include <iomanip>

namespace {
   int const sKeyLength = 32;
   int const sIterations = 1000;
   unsigned char sSalt[] = "himalayansalt";
   int const sSaltLength = sizeof(sSalt) - 1;
} // anonymous namepspace 


namespace Notes {
namespace Utils {

std::string
generatePasswordHash(std::string const& password)
{
   unsigned char hashedPasswordResult[sKeyLength];
   memset(hashedPasswordResult, 0, sKeyLength);
   int success =
      PKCS5_PBKDF2_HMAC(password.c_str(),
                        password.length(),
                        sSalt,
                        sSaltLength,
                        sIterations,
                        EVP_sha256(),
                        sKeyLength,
                        hashedPasswordResult);
   if (!success) {
      // todo throw exception
   }

   std::stringstream ss;
   for (int i = 0; i < sKeyLength; ++i) {
      ss << std::setfill('0') << std::setw(2) << std::hex << (int)hashedPasswordResult[i];
   }
   std::string finalPasswordResult = ss.str();

   return finalPasswordResult;
}

} // namespace Note
} // namespace Utils