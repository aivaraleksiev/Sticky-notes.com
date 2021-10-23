// Copyright 2021
// Author: Ayvar Aleksiev

#pragma once

#include "utils/Utils.h"
#include "AuthenticationManager.h"
#include <cassert>

#include <nlohmann/json.hpp>
#include <restinio/all.hpp>
#include <restinio/helpers/http_field_parsers/basic_auth.hpp>

using nlohmann::json;

namespace Notes {

//
// LoginEndpoint class definition.
//
class LoginEndpoint
{
public:
	LoginEndpoint()
	{
		_router = std::make_shared<express_router_t>();
	}
	auto createLoginEndpointRequestHandler();

private:
	void handlePostRequests();
	void handleDeleteRequests();

	// Helper func to identify if the client want's to create a new user.
	bool signUpQueryParamExists(auto queryParams);

	using express_router_t = restinio::router::express_router_t<>;
	std::shared_ptr<express_router_t> _router;
};

bool
LoginEndpoint::signUpQueryParamExists(auto queryParams)
{
	auto const signUpQueryParam = restinio::opt_value<std::string>(queryParams, "signUp");
	return signUpQueryParam && (*signUpQueryParam == "true");
}

void
LoginEndpoint::handlePostRequests()
{
	_router->http_post(
		"/login",
		[this](auto request, auto) {
			json outputArray;
			restinio::http_status_line_t status_line = restinio::status_ok();

			std::string username, password;
			Utils::parseBasicAuth(request->header(), username, password);
			
			if (!username.empty() && !password.empty()) {
				if (signUpQueryParamExists(restinio::parse_query(request->header().query()))) {
					AuthenticateionManager::getInstance()->createUser(username, password);
				} else {
					auto result = AuthenticateionManager::getInstance()->authenticateUser(username, password);
					if (!result) {
						// todo throw specific exception from function authenticateUser
						status_line = restinio::status_not_found();
					}
				}
			} else {
				status_line = restinio::status_bad_request();
			}

			Utils::init_response(request->create_response(status_line))
				.append_header(restinio::http_field::content_type, "text/json; charset=utf-8")
				.set_body(outputArray.dump(3))
				.done();

			return restinio::request_accepted();
		});
}

void
LoginEndpoint::handleDeleteRequests()
{
	_router->http_delete(
		R"(/login/:user)",
		[this](auto request, auto params) mutable {
			restinio::http_status_line_t status_line = restinio::status_no_content();
			
			std::string username, password;
			Utils::parseBasicAuth(request->header(), username, password);

			auto userNameParam = restinio::cast_to<std::string>(params["user"]);
			assert(username == userNameParam);
			if (!username.empty() && !password.empty()) {
				AuthenticateionManager::getInstance()->deleteUser(username, password);
			}
			
			json noContentOutput;
			Utils::init_response(request->create_response(status_line))
				.append_header(restinio::http_field::content_type, "text/json; charset=utf-8")
				.set_body(noContentOutput.dump(3))
				.done();

			return restinio::request_accepted();
		});
}

auto
LoginEndpoint::createLoginEndpointRequestHandler()
{
	handlePostRequests();
	handleDeleteRequests();

	return[handler = std::move(_router)](const auto& request) {
		return (*handler)(request);
	};

}

} // namspace Notes
