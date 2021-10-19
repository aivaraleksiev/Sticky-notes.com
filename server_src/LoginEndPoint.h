// Copyright 2021
// Author: Ayvar Aleksiev

#pragma once

#include "utils/Utils.h"

#include <restinio/all.hpp>
#include <restinio/helpers/http_field_parsers/basic_auth.hpp>

namespace Notes {

class LoginEndpoint
{
public:
	LoginEndpoint()
	{
		_router = std::make_shared<express_router_t>();
	}
	auto createLoginEndpointRequestHandler();

private:
	using express_router_t = restinio::router::express_router_t<>;
	std::shared_ptr<express_router_t> _router;
};

//
// LoginEndpoint class definition.
//

auto
LoginEndpoint::createLoginEndpointRequestHandler()
{
	_router->http_get(
		"/login",
		[](auto req, auto) {
			///
			using namespace restinio::http_field_parsers;

			const auto opt_field = req->header().opt_value_of(restinio::http_field::authorization);
			std::string username;
			std::string password;
			if (opt_field) {
				const auto parsed_field = authorization_value_t::try_parse(*opt_field);
				if (parsed_field) {
					if ("basic" == parsed_field->auth_scheme) {
						using namespace restinio::http_field_parsers::basic_auth;
						const auto basic_params = try_extract_params(*parsed_field);
						if (basic_params) {
							username = basic_params->username;
							password = basic_params->password;
						// Do something with username and password.
						}
					}
					else if ("bearer" == parsed_field->auth_scheme) {
						// Dealing with bearer authentification.
					}
					else {
						// Other authentification schemes.
					}
				}
			}

			std::stringstream ostr;
			ostr << "<html>"
				<< "<body><h1>Hello from the sticky-notes Web Serve LOGIN PAGE:"
				<< "\n username: " << username << " password: " << password <<" </h1></body>"
				<< "</html>";

			Utils::init_response(req->create_response())
				.append_header(restinio::http_field::content_type, "text/html; charset=utf-8")
				.set_body(ostr.str())
				.done();

			return restinio::request_accepted();
		});

	return[handler = std::move(_router)](const auto& req) {
		return (*handler)(req);
	};

}

} // namspace Notes
