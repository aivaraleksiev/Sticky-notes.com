// Copyright 2021
// Author: Ayvar Aleksiev

#pragma once

#include "../utils/Utils.h"
#include "../utils/HttpException.h"
#include "../auth/AuthenticationManager.h"
#include "../auth/Authorization.h"
#include "../database/SQLiteDBConnection.h"

#include <cassert>
#include <nlohmann/json.hpp>
#include <restinio/all.hpp>

using nlohmann::json;

namespace Notes {

//
// Class handling user endpoint.
// Look at REST API documentation for usage description.
//
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
	void setDbConnection(std::shared_ptr<SQLiteDBConnection> const& dbService) {
		_dbService = dbService;
	}
private:
	void handlePostRequests();
	void handleDeleteRequests();

	using express_router_t = restinio::router::express_router_t<>;
	std::shared_ptr<express_router_t> _router = std::make_shared<express_router_t>();
        std::shared_ptr<SQLiteDBConnection> _dbService {nullptr};
};

//
// UserEndpoint class definition.
//

void
UserEndpoint::handlePostRequests()
{
	_router->http_post(
		"/api/v1/user/login",
		[this](auto request, auto) {
			try {
				std::string username, password;
				Utils::parseBasicAuth(request->header(), username, password);
			   
			        auto isAuthenticated = AuthenticationManager::getInstance()->authenticateUser(username, password);
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
		"/api/v1/user/signUp",
		[this](auto request, auto) {
			try {
				std::string username, password;
				Utils::parseBasicAuth(request->header(), username, password);

				AuthenticationManager::getInstance()->createUser(username, password);
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

	_router->http_post(
		"/api/v1/:username/passwordChange",
		[this](auto request, auto params) {
			try {
				json inputArray = json::parse(request->body());
				if (inputArray.is_null()) {
					throw Utils::HttpException(restinio::status_bad_request(), "Missing request body.");
				}
				std::string oldPassword;
				std::string newPassword;
				for (auto const& obj : inputArray) {
					obj.at("oldPassword").get_to(oldPassword);
					obj.at("newPassword").get_to(newPassword);
				}
				AuthenticationManager::getInstance()->changeUserPassword(
					restinio::cast_to<std::string>(params["username"]),
					oldPassword,
					newPassword);

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

void
UserEndpoint::handleDeleteRequests()
{
	_router->http_delete(
		R"(/api/v1/user/:username)",
		[this](auto request, auto params) mutable {
			try {
				std::string username, password;
				Utils::parseBasicAuth(request->header(), username, password);

				auto userNameParam = restinio::cast_to<std::string>(params["username"]);
				if (userNameParam != username) {
					throw Utils::HttpException(restinio::status_bad_request(), "Username missmatch.");
				}
			   AuthenticationManager::getInstance()->deleteUser(username, password);				

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
