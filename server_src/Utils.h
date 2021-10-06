#pragma once

#include <restinio/all.hpp>

// helper func
template < typename RESP >
RESP
init_response(RESP resp)
{
   resp.append_header(restinio::http_field::server, "Sticky-notes.com REST server version 0");
   resp.append_header_date_field();

   return resp;
}