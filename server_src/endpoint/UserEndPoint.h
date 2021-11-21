// Copyright 2021
// Author: Ayvar Aleksiev

#pragma once

#include "../utils/Utils.h"
#include "../utils/HttpException.h"
#include "../auth/AuthenticationManager.h"
#include "../auth/Authorization.h"
#include <cassert>

#include <nlohmann/json.hpp>
#include <restinio/all.hpp>

using nlohmann::json;

namespace Notes {

//
// Class handling user endpoint.
// Look at REST API documentation for usage description.
//
// // todo may be add token in a set-cookie instead of authorization header?
class UserEndpoint
{
public:
	// Access to the singleton instance of this class.
	static UserEndpoint* getInstance()
	{
		static std::unique_ptr<UserEndpoint> sManagerPtr(
			std::make_unique<UserEndpoint>());
		return sManagerPtr.get();
	}

	auto createUserEndpointRequestHandler();
private:
	void handlePostRequests();
	void handleDeleteRequests();

	using express_router_t = restinio::router::express_router_t<>;
	std::shared_ptr<express_router_t> _router = std::make_shared<express_router_t>();
};

//
// UserEndpoint class definition.
//

void
UserEndpoint::handlePostRequests()
{
	_router->http_post(
		"/user/login",
		[this](auto request, auto) {
			try {
				std::string username, password;
				Utils::parseBasicAuth(request->header(), username, password);
			   
			   auto isAuthenticated = AuthenticateionManager::getInstance()->authenticateUser(username, password);
			   if (!isAuthenticated) {
					throw Utils::HttpException(restinio::status_bad_request(), "Invalid username or password.");
			   }
				std::string accessToken = Authorization::generateAccessToken(username);

				Utils::createNoContentResponse(request)
			   	.append_header(restinio::http_field::authorization, accessToken)
			   	.done();
			}
			catch (Utils::HttpException const& exc) {
				Utils::createErrorResponse(request, exc).done();
			}
			catch (std::exception const& exc) {
				Utils::HttpException exception(restinio::status_internal_server_error(), exc.what());
				Utils::createErrorResponse(request, exception).done();
			}

			return restinio::request_accepted();
		});

	_router->http_post(
		"/user/signUp",
		[this](auto request, auto) {
			try {
				std::string username, password;
				Utils::parseBasicAuth(request->header(), username, password);

				AuthenticateionManager::getInstance()->createUser(username, password);
				std::string accessToken = Authorization::generateAccessToken(username);

				NoteManager::getInstance()->addUserNoteBoard(username);

				Utils::createNoContentResponse(request)
					.append_header(restinio::http_field::authorization, accessToken)
					.done();
			}
			catch (Utils::HttpException const& exc) {
				Utils::createErrorResponse(request, exc).done();
			}
			catch (std::exception const& exc) {
				Utils::HttpException exception(restinio::status_internal_server_error(), exc.what());
				Utils::createErrorResponse(request, exception).done();
			}

			return restinio::request_accepted();
		});
}

void
UserEndpoint::handleDeleteRequests()
{
	_router->http_delete(
		R"(/user/:username)",
		[this](auto request, auto params) mutable {
			try {
				std::string username, password;
				Utils::parseBasicAuth(request->header(), username, password);

				auto userNameParam = restinio::cast_to<std::string>(params["username"]);
				if (userNameParam != username) {
					throw Utils::HttpException(restinio::status_bad_request(), "Username missmatch.");
				}
			   AuthenticateionManager::getInstance()->deleteUser(username, password);				

				Utils::createNoContentResponse(request).done();
			}
			catch (Utils::HttpException const& exc) {
				Utils::createErrorResponse(request, exc).done();
			}
			catch (std::exception const& exc) {
				Utils::HttpException exception(restinio::status_internal_server_error(), exc.what());
				Utils::createErrorResponse(request, exception).done();
			}
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
