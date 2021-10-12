// Copyright 2021
// Author: Ayvar Aleksiev

#pragma once

#include <restinio/all.hpp>

namespace Notes {
namespace Utils {

using UID = size_t;

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
class UIDGenerator
{
public:
   static size_t generateUID();
};

} // namespace Utils
} // namespace Notes