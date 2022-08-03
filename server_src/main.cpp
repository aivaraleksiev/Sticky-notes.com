// Copyright 2021
// Author: Ayvar Aleksiev

#include "endpoint/NotesEndpoint.h"
#include "endpoint/MainEndpoint.h"
#include "endpoint/UserEndpoint.h"
#include "endpoint/InvalidEndpoint.h"
#include "utils/Logger.h"

#include <boost/program_options.hpp>
#include <restinio/all.hpp>
#include <restinio/sync_chain/growable_size.hpp>
#include <restinio/tls.hpp>

#include <iostream>

using namespace boost::program_options;

int main(int argc, char* argv[])
{
   try
   {
      /*
       * Program options.
       */
      std::string certDir;
      options_description desc{ "Options" };
      desc.add_options()
         (",h", "Help screen.")
         ("certDir", value<std::string>(&certDir), "[Required] Absolute path to certificate directory.");

      variables_map varMap;
      store(parse_command_line(argc, argv, desc), varMap);
      notify(varMap);
      if ((argc < 2) || (varMap.count("help")) || (varMap.count("certDir") < 0)) {
         std::cout << desc << '\n';
         return 0;
      }

      /*
       * Server configuration.
       */
      using traits_t =
         restinio::single_thread_tls_traits_t<
         restinio::asio_timer_manager_t,
         Notes::Utils::Logger,
         restinio::sync_chain::growable_size_chain_t<>>;
       
      restinio::sync_chain::growable_size_chain_t<>::builder_t chain_builder;
      chain_builder.add(Notes::NotesEndpoint::getInstance()->createNoteEndpointRequestHandler());
      chain_builder.add(Notes::MainEndpoint::getInstance()->createMainEndpointRequestHandler());
      chain_builder.add(Notes::UserEndpoint::getInstance()->createUserEndpointRequestHandler());
      // The invalid endpoint handler should be added last, otherwise all handlers are returning bad request.
      chain_builder.add(Notes::InvalidEndpoint::getInstance()->createInvalidEndpointRequestHandler());

      auto asyncServer =
         restinio::run_async(
            restinio::own_io_context(),
            restinio::server_settings_t< traits_t >{}
                 .port(9066)
                 .address("localhost")
                 .logger(Notes::Utils::createLogger("restinio"))
                 .request_handler(chain_builder.release())
                 .tls_context(std::move(Notes::Utils::createTlsContext(std::move(certDir)))),
            // Threadpool size for the HTTP server.
            std::thread::hardware_concurrency());

      while (true) {
         // Keep program alive.
      }
   }
   catch (const std::exception& ex)
   {
      std::cerr << "Failed to start server. Error: " << ex.what() << std::endl;
   }

   return 0;
}

