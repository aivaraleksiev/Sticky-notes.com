// Copyright 2021
// Author: Ayvar Aleksiev

#include "Authorization.h"

namespace {
std::string const sSecret_key = "seasalt"; // todo user public/private key instead
std::string const sIssuer = "sticky-notes.com";
std::string const sUserKey = "user";
}

namespace Notes{

std::string
Authorization::generateAccessToken(std::string username)
{
   std::string accessToken = jwt::create()
      .set_issuer(sIssuer)
      .set_type("JWS")
      .set_payload_claim(sUserKey, jwt::claim(username))
      .set_issued_at(std::chrono::system_clock::now())
      .set_expires_at(std::chrono::system_clock::now() + std::chrono::seconds{ 300 })
      .sign(jwt::algorithm::hs256{ sSecret_key });

   return accessToken;
}

void
Authorization::verifyAccessToken(std::string const& token, std::string& username)
{
   try {
      auto decodedToken = jwt::decode(token);
      jwt::verify()
         .allow_algorithm(jwt::algorithm::hs256{ sSecret_key })
         .with_issuer(sIssuer)
         .not_before_leeway(std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()))
         .verify(decodedToken);
      // todo add additional checks 

      auto claimsMap = decodedToken.get_payload_claims();
      auto userIt = claimsMap.find(sUserKey);
      if (userIt != claimsMap.end()) {
         username = userIt->second.as_string();
         // todo call AuthenticateionManager::authenticateUser
      }

   }
   catch (std::exception const& e) {
      // todo remove try catch from here. Move it outside function. Add logger
      std::cout << e.what();
   }
}

} // namespace Notes