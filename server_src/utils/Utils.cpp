// Copyright 2021
// Author: Ayvar Aleksiev

#include "Utils.h"
#include "HttpException.h"
#include <restinio/helpers/http_field_parsers/basic_auth.hpp>
#include <restinio/helpers/http_field_parsers/bearer_auth.hpp>

#include <string>

namespace {
	std::atomic<size_t> sUid { 0 };
	// todo describe this in how to build software.
	std::string sCaCertPath("D:/My Projects/Github/Sticky-notes.com/server_src/certificates/cacert.pem");
	std::string sPrivateKey("D:/My Projects/Github/Sticky-notes.com/server_src/certificates/privkey.pem");

} // anonymous namespace 

namespace Notes {
namespace Utils {

UID generateUID()
{
   return ++sUid;
}

void
parseBasicAuth(
	restinio::http_request_header_t const& header, 
	std::string& username, 
	std::string& password)
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
			throw HttpException(restinio::status_bad_request(), "Invalid content in authorization header.");
	   }
	}
	else {
		throw HttpException(restinio::status_unauthorized(), "Missing Content in authorization header.");
	}
	if (username.empty() || password.empty()) {
		throw HttpException(restinio::status_bad_request(), "Invalid username or password value.");
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
		} else {
			throw HttpException(restinio::status_bad_request(), "Invalid content in authorization header.");
		}
	}
	// else
	throw HttpException(restinio::status_unauthorized(), "Missing content in authorization header.");
}

restinio::asio_ns::ssl::context
createTlsContext()
{
   restinio::asio_ns::ssl::context tls_context{ restinio::asio_ns::ssl::context::sslv23 };
   tls_context.set_options(
      restinio::asio_ns::ssl::context::default_workarounds
	  | restinio::asio_ns::ssl::context::no_sslv2);
   // todo remove hardcoded path.
   // Variant 1: use configuration file.
   // Variant 2: generate certs in memory.
	tls_context.use_certificate_chain_file(sCaCertPath);
	tls_context.use_private_key_file(sPrivateKey,
   	restinio::asio_ns::ssl::context::pem);
   return tls_context;
}

} // namespace Note
} // namespace Utils