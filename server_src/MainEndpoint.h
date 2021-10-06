#pragma once

#include "Utils.h"

#include <restinio/all.hpp>


class MainEndpoint
{
   using router_t = restinio::router::express_router_t<>;
   using express_router_t = restinio::router::express_router_t<>;

public:
   auto createMainEndpointRequestHandler()
   {
		auto router = std::make_shared<express_router_t>();

		router->http_get(
			"/",
			[](auto req, auto) {
				std::stringstream ostr;
				ostr << "<html>"
					<< "<body><h1>Hello from the sticky-notes Web Server</h1></body>"
					<< "</html>";
				std::string strResponse = ostr.str();

				init_response(req->create_response())
					.append_header(restinio::http_field::content_type, "text/html; charset=utf-8")
					.set_body(ostr.str())
					.done();

				return restinio::request_accepted();
			});

		return[handler = std::move(router)](const auto& req) {
			return (*handler)(req);
		};  
   }
};