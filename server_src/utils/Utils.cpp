// Copyright 2021
// Author: Ayvar Aleksiev

#include "Utils.h"
#include <restinio/helpers/http_field_parsers/basic_auth.hpp>
#include <restinio/helpers/http_field_parsers/bearer_auth.hpp>

namespace {
   std::atomic<size_t> sUid { 0 };
} // anonymous namepspace 

namespace Notes {
namespace Utils {

UID
UIDGenerator::generateUID()
{
   return ++sUid;
}


void
parseBasicAuth(restinio::http_request_header_t const& header, std::string& username, std::string& password)
{
	using namespace restinio::http_field_parsers;
	username.clear();
	password.clear();

	auto const authorizationHeaderOpt = header.opt_value_of(restinio::http_field::authorization);
	if (authorizationHeaderOpt) {
		const auto authorizationField = authorization_value_t::try_parse(*authorizationHeaderOpt);
		if (authorizationField && (authorizationField->auth_scheme == "basic")) {
			const auto basic_params = basic_auth::try_extract_params(*authorizationField);
			if (basic_params) {
				username = basic_params->username;
				password = basic_params->password;
			}
		}
	   else {
	   	// todo did not parse correctly or Other authentification schemes.
	   }
	}
}


std::string
extractAuthToken(restinio::http_request_header_t const& header)
{
	using namespace restinio::http_field_parsers;	

	auto const authorizationHeaderOpt = header.opt_value_of(restinio::http_field::authorization);
	if (authorizationHeaderOpt) {
		const auto authorizationField = authorization_value_t::try_parse(*authorizationHeaderOpt);
		if (authorizationField && (authorizationField->auth_scheme == "bearer")) {
			const auto bearer_params = bearer_auth::try_extract_params(*authorizationField);
		   if (bearer_params) {
		   	return bearer_params->token;
		   }
		}
	   else {
	   	// todo did not parse correctly or Other authentification schemes.
	   }
	}
}
} // namespace Note
} // namespace Utils