// Copyright 2021
// Author: Ayvar Aleksiev

#pragma once

#include <restinio/all.hpp>

namespace Notes {
namespace Utils {

// helper func
template < typename RESP >
RESP
init_response(RESP resp)
{
   resp.append_header(restinio::http_field::server, "Sticky-notes.com REST server version 0");
   resp.append_header_date_field();

   return resp;
}

class UIDGenerator
{
public:
   static size_t generateUID();
};

} // namespace Utils
} // namespace Notes