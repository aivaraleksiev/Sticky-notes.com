// Copyright 2021
// Author: Ayvar Aleksiev

#pragma once

#include "HttpException.h"
#include <restinio/all.hpp>
#include <restinio/tls.hpp>

namespace Notes {
namespace Utils {

using UID = size_t;
#define INVALID_UID UID(0)

// Unique identifier generator.
// invalid uid == 0.
UID generateUID();

// Helper function creating response for generic use.
auto
createGenericResponse(auto resp)
{
   resp.append_header(restinio::http_field::server, "sticky-notes.com");
   resp.append_header_date_field();
   return resp;
}

// Helper function.
auto
createNoContentResponse(auto const& request)
{
   return createGenericResponse(request->create_response(restinio::status_no_content()))
      .append_header(restinio::http_field::server, "sticky-notes.com")
      .append_header_date_field()
      .append_header(restinio::http_field::content_type, "text/json; charset=utf-8");
}

// Helper function.
auto
createErrorResponse(auto const&  request, auto const& exception)
{
   return createGenericResponse(request->create_response(exception.getHttpStatusLine()))
      .append_header(restinio::http_field::server, "sticky-notes.com")
      .append_header_date_field()
      .append_header(restinio::http_field::content_type, "text/json; charset=utf-8")
      .set_body(json(exception).dump(3));      
}

/* Parse authorization header searching for basic authentication.
 
   @param[in] header The HTTP header from which we are searching for basic authentication.
   @param[out] username The username value contained in authorization header.
   @param[out] password The password value contained in authorization header.
   @throws
*/
void 
parseBasicAuth(
   restinio::http_request_header_t const& header,
   std::string& username,
   std::string& password);

/* Extract encoded token from authorization header.

   @param[in] header The HTTP header from which we are searching for token.
   @throws
   @returns encoded token.
*/
std::string
extractAuthToken(restinio::http_request_header_t const& header);


/* Creates tls context. Needed to secure server using SSL/TLS.

   @throws
   @returns tls context
*/
restinio::asio_ns::ssl::context createTlsContext();

} // namespace Utils
} // namespace Notes