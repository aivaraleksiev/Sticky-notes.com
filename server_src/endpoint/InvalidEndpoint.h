// Copyright 2021
// Author: Ayvar Aleksiev

#pragma once

#include "../engine/NoteManager.h"
#include "../auth/Authorization.h"
#include "../utils/Utils.h"
#include "../utils/HttpException.h"


#include <nlohmann/json.hpp>
#include <restinio/all.hpp>

using nlohmann::json;

namespace Notes {

// Class handling invalid endpoint.
// Whenever there is a invalid endpoint request, we will handle it showing bad request.
class InvalidEndpoint
{
public:
   // Access to the singleton instance of this class.
   static InvalidEndpoint* getInstance()
   {
      static std::unique_ptr<InvalidEndpoint> sManagerPtr(
         std::make_unique<InvalidEndpoint>());
      return sManagerPtr.get();
   }
   void handleInvalidRequests();
   auto createInvalidEndpointRequestHandler();

private:
   using express_router_t = restinio::router::express_router_t<>;

   std::shared_ptr<express_router_t> _router = std::make_shared<express_router_t>();
};

//
// InvalidEndpoint class definition.
//

void
InvalidEndpoint::handleInvalidRequests()
{
   _router->non_matched_request_handler(
      [](auto request) {
         
         std::stringstream ostr;
         ostr << "<html>"
              << "<body><h1>Oops! Bad request! Are you sure you know where are you going?</h1></body>"
              << "</html>";

         Utils::createGenericResponse(request->create_response(restinio::status_bad_request()))
            .append_header(restinio::http_field::content_type, "text/html; charset=utf-8")
            .set_body(ostr.str())
            .done();
         return restinio::request_not_handled();
      });
}

auto
InvalidEndpoint::createInvalidEndpointRequestHandler()
{

   handleInvalidRequests();
   return[handler = std::move(_router)](const auto& req) {
      return (*handler)(req);
   };
}

} //namspace Notes
