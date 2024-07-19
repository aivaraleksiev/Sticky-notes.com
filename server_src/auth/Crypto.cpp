// Copyright 2021
// Author: Ayvar Aleksiev

#include "../../utils/HttpException.h"

#include <openssl/evp.h>
#include <sstream>
#include <iomanip>

namespace {
   unsigned char sSalt[] = "himalayansalt";
   int const sSaltLength = sizeof(sSalt) - 1;
   int const sKeyLength = 32;
   int const sIterations = 1000;
} // anonymous namepspace 

namespace Notes {
namespace Utils {

std::string
generatePasswordHash(std::string const& password)
{
   std::array<unsigned char, sKeyLength> hashedPasswordResult{};
   int success =
      PKCS5_PBKDF2_HMAC(password.c_str(),
                        password.length(),
                        sSalt,
                        sSaltLength,
                        sIterations,
                        EVP_sha256(),
                  hashedPasswordResult.size(),
                         hashedPasswordResult.data());
   if (!success) {
      throw Utils::HttpException(restinio::status_internal_server_error(), "Could not generate password hash.");
   }

   std::stringstream ss;
   for (auto const& value : hashedPasswordResult) {
      ss << std::setfill('0') << std::setw(2) << std::hex << static_cast<int>(value);
   }
   std::string finalPasswordResult = ss.str();

   return finalPasswordResult;
}

} // namespace Note
} // namespace Utils
