// Copyright 2021
// Author: Ayvar Aleksiev

#pragma once

#include "utils/Utils.h"

#include <restinio/all.hpp>

namespace Notes {

class MainEndpoint
{
public:
	MainEndpoint()
	{
		_router = std::make_shared<express_router_t>();
	}
	auto createMainEndpointRequestHandler();

private:
	using express_router_t = restinio::router::express_router_t<>;
	std::shared_ptr<express_router_t> _router;
};

//
// MainEndpoint class definition.
//

auto
MainEndpoint::createMainEndpointRequestHandler()
{
	_router->http_get(
		"/",
		[](auto req, auto) {
			std::stringstream ostr;
			ostr << "<html>"
				<< "<body><h1>Hello from the sticky-notes Web Server</h1></body>"
				<< "</html>";
			restinio::http_status_line_t status_line = restinio::status_ok();
			Utils::init_response(req->create_response(status_line))
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