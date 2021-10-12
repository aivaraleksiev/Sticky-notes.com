// Copyright 2021
// Author: Ayvar Aleksiev

#pragma once

#include "NoteBoard.h"
#include "Utils.h"

#include <restinio/all.hpp>

// Currently the API is not fully implemented.
// 
// Use case scenario for testing:
// TODO remove verbs from requests.
// GET /api/v1/notes
// POST /api/v1/notes/addNote/TitleName
// GET /api/v1/notes

namespace Notes {

class NotesEndpoint
{

public:
   auto createNoteEndpointRequestHandler();
};

auto
NotesEndpoint::createNoteEndpointRequestHandler()
{
   using express_router_t = restinio::router::express_router_t<>;
   auto router = std::make_shared<express_router_t>();
   // temporary place
   std::shared_ptr<NoteBoard> localBoard = std::make_shared<NoteBoard>();

   router->http_get( // todo we can send them the api documentation file. or pointtoa link
      "/api/v1/usage",
      [](auto req, auto) {
         std::stringstream ostr;
         ostr << "<html>"
            << "<body><h1>Here will be the API usage webpage.</h1></body>"
            << "</html>";
         std::string strResponse = ostr.str();
         Utils::init_response(req->create_response())
            .append_header(restinio::http_field::content_type, "text/html; charset=utf-8")
            .set_body(ostr.str())
            .done();

         return restinio::request_accepted();
      });

   router->http_get(
      "/api/v1/notes", // return all notes
      [localBoard](auto req, auto) mutable {

         // ayvar
         //restinio::request_handle_t req2 = req;
         //auto requestBody = req2->body();
         //restinio::http_request_header_t header = req->header();
         // ayvar end

         std::string noteTitle("empty");
         if (!localBoard->getNotes().empty()) {
            noteTitle = localBoard->getNotes().begin()->second.getTitle();
         }
         std::string json = fmt::format("{{ \"Note title\": \"{}\" }}", noteTitle);

         Utils::init_response(req->create_response())
            .append_header(restinio::http_field::content_type, "text/json; charset=utf-8")
            .set_body(json)
            .done();

         return restinio::request_accepted();
      });

   router->http_post( // ayvar comment: remove verb. Use only nouns
      "/api/v1/notes/addNote/:title",
      [localBoard](auto req, auto params) mutable {

         Note newNote;
         restinio::string_view_t view = params["title"];
         std::string titleName(view.data(), view.size());
         newNote.setTitle(titleName);
         localBoard->createNote(newNote);

         std::string json = fmt::format("{{ \"Note title\": \"{}\" }}", newNote.getTitle());

         Utils::init_response(req->create_response())
            .append_header(restinio::http_field::content_type, "text/json; charset=utf-8")
            .set_body(json)
            .done();

         return restinio::request_accepted();
      });

   return[handler = std::move(router)](const auto& req) {
      return (*handler)(req);
   };
}

} //namspace Notes