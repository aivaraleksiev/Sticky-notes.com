// Copyright 2021
// Author: Ayvar Aleksiev

#include "endpoint/NotesEndpoint.h"
#include "endpoint/MainEndpoint.h"
#include "endpoint/UserEndpoint.h"
#include "endpoint/InvalidEndpoint.h"
#include "database/SQLiteDBConnection.h"
#include "utils/Logger.h"

#include <boost/program_options.hpp>
#include <restinio/all.hpp>
#include <restinio/sync_chain/growable_size.hpp>
#include <restinio/tls.hpp>

#include <iostream>

using namespace boost::program_options;

class ProgramOptions
{
public:
   ProgramOptions() = default;
   ProgramOptions(ProgramOptions const& o) = delete;
   ProgramOptions& operator=(ProgramOptions const& o) = delete;
   bool parseCmdLine(int argc, char* argv[]) {
      options_description desc{ "Options" };
      desc.add_options()
         (",h", "Help screen.")
         ("certDir", value<std::string>(&_certDir), "[Required] Absolute path to certificate directory.");

      variables_map varMap;
      store(parse_command_line(argc, argv, desc), varMap);
      notify(varMap);
      if ((argc < 2) || (varMap.count("help")) || (varMap.count("certDir") < 0)) {
         std::cout << desc << '\n';
         return false;
      }
      return true;
   }
   // Returns Certificate directory absolute path.
   std::string getCertDir() const {
      return _certDir;
   }
private:
   std::string _certDir;
};

int main(int argc, char* argv[])
{
   try
   {
      ProgramOptions cmdLineOptHandler;
      if (!cmdLineOptHandler.parseCmdLine(argc, argv)) {
         return 0;
      }
      std::string certDir = cmdLineOptHandler.getCertDir();

      // Initialize database.
      std::shared_ptr<Notes::SQLiteDBConnection> dbServiceSPtr =
         Notes::SQLiteDBConnection::createInstance("sticky-notes.db");
      
      // Server configuration.
      using traits_t =
         restinio::single_thread_tls_traits_t<
         restinio::asio_timer_manager_t,
         Notes::Utils::Logger,
         restinio::sync_chain::growable_size_chain_t<>>;

      // Add endpoints.
      auto userEndpointUPtr = Notes::UserEndpoint::getInstance();
      userEndpointUPtr->setDbConnection(dbServiceSPtr);
      auto notesEndpointUPtr = Notes::NotesEndpoint::getInstance();
      notesEndpointUPtr->setDbConnection(dbServiceSPtr);
      
      restinio::sync_chain::growable_size_chain_t<>::builder_t chain_builder;
      chain_builder.add(notesEndpointUPtr->createNoteEndpointRequestHandler());
      chain_builder.add(Notes::MainEndpoint::getInstance()->createMainEndpointRequestHandler());
      chain_builder.add(userEndpointPtr->createUserEndpointRequestHandler());
      
      // The invalid endpoint handler should be added last, otherwise all handlers are returning bad request.
      chain_builder.add(Notes::InvalidEndpoint::getInstance()->createInvalidEndpointRequestHandler());

      auto asyncServerPtr =
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

