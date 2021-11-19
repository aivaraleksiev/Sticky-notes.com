// Copyright 2021
// Author: Ayvar Aleksiev

#include "endpoint/NotesEndpoint.h"
#include "endpoint/MainEndpoint.h"
#include "endpoint/UserEndpoint.h"
#include "utils/Logger.h"

#include <restinio/all.hpp>
#include <restinio/sync_chain/fixed_size.hpp>
#include <restinio/sync_chain/growable_size.hpp>
#include <restinio/tls.hpp>

#include <iostream>

int main()
{	
	try
	{
		using traits_t =
			restinio::single_thread_tls_traits_t<
			restinio::asio_timer_manager_t,
			Notes::Utils::Logger,
			restinio::sync_chain::growable_size_chain_t<>>;
		 
		//todo  make some refactoring and add handlers in endpoint code.
		restinio::sync_chain::growable_size_chain_t<>::builder_t chain_builder;

		chain_builder.add(Notes::NotesEndpoint::getInstance()->createNoteEndpointRequestHandler());
		chain_builder.add(Notes::MainEndpoint::getInstance()->createMainEndpointRequestHandler());
		chain_builder.add(Notes::UserEndpoint::getInstance()->createUserEndpointRequestHandler());

		restinio::run(
			restinio::on_this_thread<traits_t>()
			.port(9066)
			.address("localhost")
			.logger(Notes::Utils::createLogger("restinio"))
			.request_handler(chain_builder.release())
			.tls_context(std::move(Notes::Utils::createTlsContext())));
	}
	catch (const std::exception& ex)
	{
		std::cerr << "Failed to start server. Error: " << ex.what() << std::endl;
		return 1;
	}

	return 0;
}

