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
   void handlePutRequests();
   void handleDeleteRequests();
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
         json outputArray;
         const auto queryParams = restinio::parse_query(req->header().query());
         std::vector<Note> result;
         if (!queryParams.empty()) {
            // Handle search query.
            // example: /notes?title={string}&text={string}&color={string}
            auto const titleQueryParam = restinio::opt_value<std::string>(queryParams, "title");
            auto const textQueryParam = restinio::opt_value<std::string>(queryParams, "text");
            auto const colorQueryParam = restinio::opt_value<std::string>(queryParams, "color");

            if (titleQueryParam) {
               result = _noteBoard->searchByTitle(*titleQueryParam);
            }
            if (textQueryParam) {
               auto searchResult = _noteBoard->searchByText(*textQueryParam);
               result.reserve(result.size() + searchResult.size());
               result.insert(
                  result.end(),
                  std::make_move_iterator(searchResult.begin()),
                  std::make_move_iterator(searchResult.end())
               );
            }
            if (colorQueryParam) {
               Color color = toColor(*colorQueryParam);
               auto searchResult = _noteBoard->searchByColor(color);
               result.reserve(result.size() + searchResult.size());
               result.insert(
                  result.end(),
                  std::make_move_iterator(searchResult.begin()),
                  std::make_move_iterator(searchResult.end())
               );
            }
            auto removeDuplicates = [&result]() {
               auto cmpLess =
                  [](auto const& lhsNote, auto const& rhsNote) {
                  return lhsNote.getUID() < rhsNote.getUID();
               };
               std::sort(result.begin(), result.end(), cmpLess);
               auto predicate =
                  [](auto const& lhsNote, auto const& rhsNote) {
                     return lhsNote.getUID() == rhsNote.getUID();
               };
               auto last = std::unique(result.begin(), result.end(), predicate);
               result.erase(last, result.end());
            };
            removeDuplicates();
         }
         else {
            auto notesMap = _noteBoard->getNotes();
            for (auto const& [id, noteValue] : notesMap) {
               result.push_back(noteValue);
            }
         }

         for (auto const& note : result) {
            outputArray.push_back(note);
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
}

void
NotesEndpoint::handlePostRequests()
{
   _router->http_post(
      "/api/v1/notes",
      [this](auto req, auto params) mutable {
         restinio::http_status_line_t status_line = restinio::status_ok();
         json output;
         json jArray;
         json inputArray = json::parse(req->body());
         for (auto const& obj : inputArray) {
            Note newNote;
            std::string readInput;
            obj.at("title").get_to(readInput);
            newNote.setTitle(readInput);
            obj.at("text").get_to(readInput);
            newNote.setText(readInput);
            Color readColor;
            obj.at("color").get_to<Color>(readColor);
            if (readColor != Color::invalid) {
               newNote.setColor(readColor);
            }
            jArray.push_back(_noteBoard->createNote(newNote));
         }
         output["noteId"] = jArray;
         status_line = restinio::status_created();
         Utils::init_response(req->create_response(status_line))
            .append_header(restinio::http_field::content_type, "text/json; charset=utf-8")
            .set_body(output.dump(3))
            .done();

         return restinio::request_accepted();
      });
}

void
NotesEndpoint::handlePutRequests()
{

   // TODO https://bestofcpp.com/repo/Stiffstream-restinio-cpp-network
}

void
NotesEndpoint::handleDeleteRequests()
{
   _router->http_delete(
      R"(/api/v1/notes/:noteId(\d+))",
      [this](auto req, auto params) mutable {
         restinio::http_status_line_t status_line = restinio::status_no_content();
         auto noteId = restinio::cast_to<UID>(params["noteId"]);
         auto note = _noteBoard->deleteNote(noteId);
         json outputObj;
         Utils::init_response(req->create_response(status_line))
            .append_header(restinio::http_field::content_type, "text/json; charset=utf-8")
            .set_body(outputObj.dump(3))
            .done();

         return restinio::request_accepted();
      });
}

void
NotesEndpoint::handleInvalidRequests()
{
   _router->non_matched_request_handler(
      [](auto req) {
         restinio::http_status_line_t status_line = restinio::status_bad_request();

         return
            Utils::init_response(req->create_response(status_line))
               .append_header(restinio::http_field::content_type, "text/json; charset=utf-8")
               .done();
      });
}
auto
NotesEndpoint::createNoteEndpointRequestHandler()
{
   handleGetRequests();
   handlePostRequests();
   handleDeleteRequests();
   handleInvalidRequests();
   return[handler = std::move(_router)](const auto& req) {
      return (*handler)(req);
   };
}

} //namspace Notes