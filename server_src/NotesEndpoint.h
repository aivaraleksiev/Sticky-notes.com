// Copyright 2021
// Author: Ayvar Aleksiev

#pragma once

#include "NoteBoard.h"
#include "Utils.h"

#include <nlohmann/json.hpp>
#include <restinio/all.hpp>

using nlohmann::json;

namespace Notes {

class NotesEndpoint
{
public:
   NotesEndpoint();
   // Handle GET requests:
   // GET	/notes
   // GET  /notes/{noteId}
   // GET	/notes?title={string}&text={string}&color={string}
   void handleGetRequests();
   // todo optional values https://bestofcpp.com/repo/Stiffstream-restinio-cpp-network for POST
   void handlePostRequests();
   // todo optional values https://bestofcpp.com/repo/Stiffstream-restinio-cpp-network for PUT
   void handlePutRequests() {};
   void handleInvalidRequests();
   auto createNoteEndpointRequestHandler();

private:

   std::shared_ptr<NoteBoard> _noteBoard;

   using express_router_t = restinio::router::express_router_t<>;

   std::shared_ptr<express_router_t> _router;
};

//
// NotesEndpoint class definition.
//

NotesEndpoint::NotesEndpoint()
{
   _noteBoard = std::make_shared<NoteBoard>();
   _router = std::make_shared<express_router_t>();
}

void
NotesEndpoint::handleGetRequests()
{
   // Return all notes.
   _router->http_get(
      "/api/v1/notes",
      [this](auto req, auto) mutable {
         restinio::http_status_line_t status_line = restinio::status_ok();
         auto notesMap = _noteBoard->getNotes();
         json outputArray;
         for (auto const& [id, noteValue] : notesMap) {
            outputArray.push_back(noteValue);
         }
         if (outputArray.is_null()) {
            // status code: 204
            status_line = restinio::status_no_content();
         }
         Utils::init_response(req->create_response(status_line))
            .append_header(restinio::http_field::content_type, "text/json; charset=utf-8")            
            .set_body(outputArray.dump(3))
            .done();

         return restinio::request_accepted();
      });

   // Return note with specified id.
   _router->http_get(
      R"(/api/v1/notes/:noteId(\d+))",
      [this](auto req, auto params) mutable { 
         restinio::http_status_line_t status_line = restinio::status_ok();
         auto noteId = restinio::cast_to<UID>(params["noteId"]);
         auto note = _noteBoard->getNote(noteId);
         json outputObj;
         if (note.getUID() != INVALID_UID) {
            to_json(outputObj, note);
         }
         if (outputObj.is_null()) {
            // status code: 204
            status_line = restinio::status_no_content();
         }
         Utils::init_response(req->create_response(status_line))
            .append_header(restinio::http_field::content_type, "text/json; charset=utf-8")            
            .set_body(outputObj.dump(3))
            .done();

         return restinio::request_accepted();
      });

      // todo https://bestofcpp.com/repo/Stiffstream-restinio-cpp-network
      // todo https://github.com/nlohmann/json#json-pointer-and-json-patch
      _router->http_get(
         "/api/v1/notes?title={string}&text={string}&color={string}", // return note with note Id
         [this](auto req, auto params) mutable {
            restinio::http_status_line_t status_line = restinio::status_ok();
            const auto qp = restinio::parse_query(req->header().query());
            //auto noteId = restinio::cast_to<UID>(params["noteId"]);
            //auto note = _noteBoard->getNote(noteId);
            json outputObj;
            //to_json(outputObj, note);
      
            Utils::init_response(req->create_response())
               .append_header(restinio::http_field::content_type, "text/json; charset=utf-8")
               .set_body(outputObj.dump(3))
               .done();
      
            return restinio::request_accepted();
         });
}

void
NotesEndpoint::handlePostRequests()
{
   _router->http_post( // ayvar comment: remove verb. Use only nouns
      "/api/v1/notes",
      [this](auto req, auto params) mutable {

         json output;

         Utils::init_response(req->create_response())
            .append_header(restinio::http_field::content_type, "text/json; charset=utf-8")
            .set_body(output.dump(3))
            .done();

         return restinio::request_accepted();
      });
}

void
NotesEndpoint::handleInvalidRequests()
{
   _router->non_matched_request_handler(
      [](auto req) {
         return req->create_response(restinio::status_not_found()).connection_close().done();
      });
}
auto
NotesEndpoint::createNoteEndpointRequestHandler()
{
   handleGetRequests();
   handleInvalidRequests();
   return[handler = std::move(_router)](const auto& req) {
      return (*handler)(req);
   };
}

} //namspace Notes