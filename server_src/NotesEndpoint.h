#pragma once

#include "NoteBoard.h"
#include "Utils.h"

#include <restinio/all.hpp>

// Currently the API is not fully implemented.
// 
// Use case scenario for testing:
// 
// GET /api/v0/notes
// POST /api/v0/notes/addNote/TitleName
// GET /api/v0/notes


class NotesEndpoint
{
	using router_t = restinio::router::express_router_t<>;
	using express_router_t = restinio::router::express_router_t<>;

public:
	auto createNoteEndpointRequestHandler()
	{	
      auto router = std::make_shared<express_router_t>();
      // temporary place
      std::shared_ptr<NoteBoard> localBoard = std::make_shared<NoteBoard>();
      
      router->http_get(
      	"/api/v0/usage",
      	[](auto req, auto) {
      		std::stringstream ostr;
      		ostr << "<html>"
      			<< "<body><h1>Here will be the API usage webpage.</h1></body>"
      			<< "</html>";
      		std::string strResponse = ostr.str();
            init_response(req->create_response())
      			.append_header(restinio::http_field::content_type, "text/html; charset=utf-8")
      			.set_body(ostr.str())
      			.done();
      
      		return restinio::request_accepted();
      	});
      
      router->http_get(
      	"/api/v0/notes", // return all notes
      	[localBoard](auto req, auto) mutable {
      
            std::string noteTitle("empty");
            if (!localBoard->getNotes().empty()) {
               noteTitle = localBoard->getNotes().begin()->getTitle();
            }
      		std::string json = fmt::format("{{ \"Note title\": \"{}\" }}", noteTitle);
      
      		init_response(req->create_response())
      			.append_header(restinio::http_field::content_type, "text/json; charset=utf-8")
      			.set_body(json)
      	.done();
      
      		return restinio::request_accepted();
      });
      
      router->http_post( // ayvar comment: maybe http_put here.
      	"/api/v0/notes/addNote/:title", // remove verb. Use only nouns
      	[localBoard](auto req, auto params) mutable {
      		
      		Note newNote;
      		restinio::string_view_t view = params[ "title" ];
      		std::string titleName(view.data(), view.size());
      		newNote.addTitle(titleName);
      		localBoard->addNote(newNote);
      
      		std::string json = fmt::format("{{ \"Note title\": \"{}\" }}", newNote.getTitle());
      
      		init_response(req->create_response())
      			.append_header(restinio::http_field::content_type, "text/json; charset=utf-8")
      			.set_body(json)
      			.done();
      
      		return restinio::request_accepted();
      	});
      
      return[handler = std::move(router)](const auto& req) {
      	return (*handler)(req);
      };
	};
};