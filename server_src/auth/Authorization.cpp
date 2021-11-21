// Copyright 2021
// Author: Ayvar Aleksiev

#include "Authorization.h"
#include "AuthenticationManager.h"

#include "../utils/Utils.h"
#include "../utils/HttpException.h"


namespace {
std::string const sSecretKey = "seasalt";
std::string const sIssuerTag = "sticky-notes.com";
std::string const sUserKey = "user";
}

namespace Notes{

std::string
Authorization::generateAccessToken(std::string username)
{
   std::string accessToken = jwt::create()
      .set_issuer(sIssuerTag)
      .set_type("JWT")
      .set_payload_claim(sUserKey, jwt::claim(username))
      .set_issued_at(std::chrono::system_clock::now())
      .set_expires_at(std::chrono::system_clock::now() + std::chrono::seconds{ 600 })
      .sign(jwt::algorithm::hs256{ sSecretKey });

   return accessToken;
}

void
Authorization::verifyAccessToken(std::string const& token, std::string const& username)
{
   auto decodedToken = jwt::decode(token);
   jwt::verify()
      .allow_algorithm(jwt::algorithm::hs256{ sSecretKey })
      .with_issuer(sIssuerTag)
      .not_before_leeway(std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()))
      .verify(decodedToken);
   // todo add additional checks 

   auto claimsMap = decodedToken.get_payload_claims();
   auto userIt = claimsMap.find(sUserKey);
   if (userIt != claimsMap.end()) {
      if (username != userIt->second.as_string()) {
         throw Utils::HttpException(restinio::status_bad_request(), "Invalid token.");
      }
      bool foundUser = AuthenticateionManager::getInstance()->userExist(username);
      if (!foundUser) {
         throw Utils::HttpException(restinio::status_not_found(), "User not found.");
      }
   }
}

void
Authorization::verifyAccessToken(
   restinio::http_request_header_t const& header,
   std::string const& username)
{
   std::string token = Utils::extractAuthToken(header);
   Authorization::verifyAccessToken(token, username);
}


} // namespace Notes