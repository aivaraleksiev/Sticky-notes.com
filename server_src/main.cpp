// Copyright 2021
// Author: Ayvar Aleksiev

#include "NotesEndpoint.h"
#include "MainEndpoint.h"
#include "LoginEndpoint.h"
#include "utils/Logger.h"

#include <restinio/all.hpp>
#include <restinio/sync_chain/fixed_size.hpp>

#include <iostream>

int main()
{	
	try
	{		
		Notes::NotesEndpoint notesEndpoint;
		Notes::MainEndpoint mainEndpoint;
		Notes::LoginEndpoint loginEndPoint;
		auto restinioLogger = Notes::Utils::createLogger("restinio");

		using traits_t =
			restinio::traits_t<
			restinio::asio_timer_manager_t,
			Notes::Utils::Logger,
			restinio::sync_chain::fixed_size_chain_t<3>>; //todo  try with growable_size_chain_t

		restinio::run(
			restinio::on_this_thread<traits_t>()
			.port(9066)
			.address("localhost")
			.logger(std::move(restinioLogger))
			.request_handler(
				notesEndpoint.createNoteEndpointRequestHandler(),
				mainEndpoint.createMainEndpointRequestHandler(),
				loginEndPoint.createLoginEndpointRequestHandler()));
	}
	catch (const std::exception& ex)
	{
		std::cerr << "Error: " << ex.what() << std::endl;
		return 1;
	}

	return 0;
}

