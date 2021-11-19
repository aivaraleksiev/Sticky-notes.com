// Copyright 2021
// Author: Ayvar Aleksiev

#include "endpoint/NotesEndpoint.h"
#include "endpoint/MainEndpoint.h"
#include "endpoint/UserEndpoint.h"
#include "utils/Logger.h"

#include <restinio/all.hpp>
#include <restinio/sync_chain/fixed_size.hpp>
#include <restinio/tls.hpp>

#include <iostream>

int main()
{	
	try
	{		
		Notes::NotesEndpoint notesEndpoint;
		Notes::MainEndpoint mainEndpoint;
		Notes::UserEndpoint userEndPoint;
		auto restinioLogger = Notes::Utils::createLogger("restinio");

		using traits_t =
			restinio::single_thread_tls_traits_t<
			restinio::asio_timer_manager_t,
			Notes::Utils::Logger,
			restinio::sync_chain::fixed_size_chain_t<3>>; //todo  try with growable_size_chain_t

		restinio::asio_ns::ssl::context tls_context{ restinio::asio_ns::ssl::context::sslv23 };
		tls_context.set_options(
			restinio::asio_ns::ssl::context::default_workarounds
			| restinio::asio_ns::ssl::context::no_sslv2);
		// todo remove hardcoded path.
		// Variant 1: use configuration file.
		// Variant 2: generate certs in memory.
		tls_context.use_certificate_chain_file("D:/My Projects/Github/Sticky-notes.com/server_src/certificates/cacert.pem");
		tls_context.use_private_key_file("D:/My Projects/Github/Sticky-notes.com/server_src/certificates/privkey.pem",
			                             restinio::asio_ns::ssl::context::pem);

		restinio::run(
			restinio::on_this_thread<traits_t>()
			.port(9066)
			.address("localhost")
			.logger(std::move(restinioLogger))
			.request_handler(
				notesEndpoint.createNoteEndpointRequestHandler(),
				mainEndpoint.createMainEndpointRequestHandler(),
				userEndPoint.createUserEndpointRequestHandler())
			.tls_context(std::move(tls_context)));
	}
	catch (const std::exception& ex)
	{
		std::cerr << "Error: " << ex.what() << std::endl;
		return 1;
	}

	return 0;
}

