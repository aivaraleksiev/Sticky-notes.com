// Copyright 2021
// Author: Ayvar Aleksiev

#pragma once

#include <string>

#include <restinio/all.hpp>

namespace Notes {
namespace Utils {

using UID = size_t;
#define INVALID_UID UID(0)

// helper func
template < typename RESP >
RESP
init_response(RESP resp)
{
   resp.append_header(restinio::http_field::server, "sticky-notes.com REST server version 1");
   resp.append_header_date_field();
   return resp;
}

// Unique identifier Generator
// invalid uid == 0.
class UIDGenerator
{
public:
   // todo make the value unique to tackle the problem when you have a DB and need to restart the server.
   // Currently the IDs will start again from zero.
   static UID generateUID();
};


// Parse authorization header searching for basic authentication
// param[in] header
// param[out] username
// param[out] password
void 
parseBasicAuth(restinio::http_request_header_t const& header,
               std::string& username,
               std::string& password);

} // namespace Utils
} // namespace Notes