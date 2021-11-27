// Copyright 2021
// Author: Ayvar Aleksiev

#pragma once

#include <jwt-cpp/jwt.h>
#include <restinio/all.hpp>

namespace Notes {

class Authorization
{
public:
   static std::string
   generateAccessToken(std::string username);

   static void
   verifyAccessToken(std::string const& token, std::string const& username);

   static void
   verifyAccessToken(restinio::http_request_header_t const& header, std::string const& username);
};

} // namespace Notes