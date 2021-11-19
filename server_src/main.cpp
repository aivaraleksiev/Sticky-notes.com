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
		using traits_t =
			restinio::single_thread_tls_traits_t<
			restinio::asio_timer_manager_t,
			Notes::Utils::Logger,
			restinio::sync_chain::fixed_size_chain_t<3>>; //todo  try with growable_size_chain_t

		restinio::run(
			restinio::on_this_thread<traits_t>()
			.port(9066)
			.address("localhost")
			.logger(Notes::Utils::createLogger("restinio"))
			.request_handler(
				Notes::NotesEndpoint::getInstance()->createNoteEndpointRequestHandler(),
				Notes::MainEndpoint::getInstance()->createMainEndpointRequestHandler(),
				Notes::UserEndpoint::getInstance()->createUserEndpointRequestHandler())
			.tls_context(std::move(Notes::Utils::createTlsContext())));
	}
	catch (const std::exception& ex)
	{
		std::cerr << "Failed to start server. Error: " << ex.what() << std::endl;
		return 1;
	}

	return 0;
}

