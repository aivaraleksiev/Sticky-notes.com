// Copyright 2021
// Author: Ayvar Aleksiev

#pragma once

#include "utils/Utils.h"
#include "AuthenticationManager.h"
#include "Authorization.h"
#include <cassert>

#include <nlohmann/json.hpp>
#include <restinio/all.hpp>

using nlohmann::json;

namespace Notes {

//
// UserEndpoint class definition.
//
class UserEndpoint
{
public:
	UserEndpoint()
	{
		_router = std::make_shared<express_router_t>();
	}
	auto createUserEndpointRequestHandler();

private:
	void handlePostRequests();
	void handleDeleteRequests();

	using express_router_t = restinio::router::express_router_t<>;
	std::shared_ptr<express_router_t> _router;
};

void
UserEndpoint::handlePostRequests()
{
	_router->http_post(
		"/user/login",
		[this](auto request, auto) {
			json outputArray;
			restinio::http_status_line_t status_line = restinio::status_ok();

			std::string username;
			std::string password;
			Utils::parseBasicAuth(request->header(), username, password);
			std::string accessToken;
			if (!username.empty() && !password.empty()) {			
			   auto isAuthenticated = AuthenticateionManager::getInstance()->authenticateUser(username, password);
			   if (!isAuthenticated) {
			   	// todo throw specific exception from function authenticateUser
			   	status_line = restinio::status_not_found();
			   }
				accessToken = Authorization::generateAccessToken(username);
			}
			else {
				status_line = restinio::status_bad_request();
			}

			Utils::init_response(request->create_response(status_line))
				.append_header(restinio::http_field::content_type, "text/json; charset=utf-8")
				.append_header(restinio::http_field::authorization, accessToken)
				.set_body(outputArray.dump(3)) 
				.done();

			return restinio::request_accepted();
		});

	_router->http_post(
		"/user/signUp",
		[this](auto request, auto) {
			json outputArray;
			restinio::http_status_line_t status_line = restinio::status_ok();

			std::string username;
			std::string password;
			Utils::parseBasicAuth(request->header(), username, password);
			std::string accessToken;
			if (!username.empty() && !password.empty()) {
				AuthenticateionManager::getInstance()->createUser(username, password);
				accessToken = Authorization::generateAccessToken(username);

				NoteManager::getInstance()->addUserNoteBoard(username);
			}
			else {
				status_line = restinio::status_bad_request();
			}

			Utils::init_response(request->create_response(status_line))
				.append_header(restinio::http_field::content_type, "text/json; charset=utf-8")
				.append_header(restinio::http_field::authorization, accessToken)
				.set_body(outputArray.dump(3))
				.done();

			return restinio::request_accepted();
		});
}

void
UserEndpoint::handleDeleteRequests()
{
	_router->http_delete(
		R"(/user/:user)",
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
UserEndpoint::createUserEndpointRequestHandler()
{
	handlePostRequests();
	handleDeleteRequests();

	return[handler = std::move(_router)](const auto& request) {
		return (*handler)(request);
	};

}

} // namspace Notes
