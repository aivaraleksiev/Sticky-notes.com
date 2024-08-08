// Copyright 2021
// Author: Ayvar Aleksiev

#pragma once

#include "../engine/NoteBoard.h"
#include "../engine/NoteManager.h"
#include "../auth/Authorization.h"
#include "../utils/Utils.h"
#include "../utils/HttpException.h"
#include "../database/SQLiteDBConnection.h"

#include <nlohmann/json.hpp>
#include <restinio/all.hpp>

using nlohmann::json;

namespace Notes {

// Class handling notes endpoint.
// Look at REST API documentation for usage description.
//
class NotesEndpoint
{
public:
    // Access to the singleton instance of this class.
    static NotesEndpoint* getInstance()
    {
        static std::unique_ptr<NotesEndpoint> sManagerPtr(
            std::make_unique<NotesEndpoint>());
        return sManagerPtr.get();
    }

   void handleGetRequests();
   void handlePostRequests();
   void handlePutRequests();
   void handleDeleteRequests();
   auto createNoteEndpointRequestHandler();
   void setDbConnection(std::shared_ptr<SQLiteDBConnection> const& dbService) {
      NoteManager::getInstance()->setDbConnection(dbService);
   }
private:  
   // Helper function handling query params for filtered GET request.
   void handleQueryParams(
      restinio::query_string_params_t const& queryParams, 
      std::shared_ptr<NoteBoard> const& noteBoardPtr,
      std::vector<std::shared_ptr<Note>>& result);

   using express_router_t = restinio::router::express_router_t<>;
   std::shared_ptr<express_router_t> _router = std::make_shared<express_router_t>();

   std::shared_ptr<SQLiteDBConnection> _dbService {nullptr};
};

//
// NotesEndpoint class definition.
//

void
NotesEndpoint::handleQueryParams(
   restinio::query_string_params_t const& queryParams,
   std::shared_ptr<NoteBoard> const& noteBoardPtr,
   std::vector<std::shared_ptr<Note>>& result)
{
   if (queryParams.empty()) {
      return;
   }
   // Handle search query.
   // example: /notes?title={string}&text={string}&color={string}
   auto const titleQueryParam = restinio::opt_value<std::string>(queryParams, "title");
   auto const textQueryParam = restinio::opt_value<std::string>(queryParams, "text");
   auto const colorQueryParam = restinio::opt_value<std::string>(queryParams, "color");

   if (titleQueryParam) {
      result = noteBoardPtr->searchByTitle(*titleQueryParam);
   }
   if (textQueryParam) {
      auto searchResult = noteBoardPtr->searchByText(*textQueryParam);
      result.reserve(result.size() + searchResult.size());
      result.insert(
         result.end(),
         std::make_move_iterator(searchResult.begin()),
         std::make_move_iterator(searchResult.end())
      );
   }
   if (colorQueryParam) {
      Color color = toColor(*colorQueryParam);
      auto searchResult = noteBoardPtr->searchByColor(color);
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
         return lhsNote->getUID() < rhsNote->getUID();
      };
      std::sort(result.begin(), result.end(), cmpLess);
      auto predicate =
         [](auto const& lhsNote, auto const& rhsNote) {
         return lhsNote->getUID() == rhsNote->getUID();
      };
      auto last = std::unique(result.begin(), result.end(), predicate);
      result.erase(last, result.end());
   };
   removeDuplicates();
}

void
NotesEndpoint::handleGetRequests()
{
   // Return all notes or notes filtered by query params.
   _router->http_get(
      "/api/v1/:username/notes",
      [this](auto request, auto params) mutable {
         try {
            auto userName = restinio::cast_to<std::string>(params["username"]);
            Authorization::verifyAccessToken(request->header(), userName);
            auto noteBoardPtr = NoteManager::getInstance()->getUserNoteBoard(userName);

            const auto queryParams = restinio::parse_query(request->header().query());
            std::vector<std::shared_ptr<Note>> result;
            if (!queryParams.empty()) {
               handleQueryParams(queryParams, noteBoardPtr, result);
            }
            else {
               auto notesMapPtr = noteBoardPtr->getNotes();
               for (auto const& [id, noteValue] : *notesMapPtr) {
                  result.push_back(noteValue);
               }
            }
            json outputArray;
            for (std::shared_ptr<Note> const& notePtr : result) {
               outputArray.push_back(*notePtr);
            }
            restinio::http_status_line_t status_line = 
               outputArray.is_null() ? restinio::status_ok() : restinio::status_no_content();
            Utils::createGenericResponse(request->create_response(status_line))
               .append_header(restinio::http_field::content_type, "text/json; charset=utf-8")
               .set_body(outputArray.dump(3))
               .done();
         }
         catch (Utils::HttpException const& exc) {
            Utils::createErrorResponse(request, exc).done();
         }
         catch (std::exception const& exc) {
            Utils::HttpException exception(restinio::status_internal_server_error(), exc.what());
            Utils::createErrorResponse(request, exception).done();
         }
         return restinio::request_accepted();
      });

   // Returns note with specified id.
   _router->http_get(
      R"(/api/v1/:username/notes/:noteId(\d+))",
      [this](auto request, auto params) mutable {
         try {
            auto userName = restinio::cast_to<std::string>(params["username"]);
            Authorization::verifyAccessToken(request->header(), userName);
            auto noteBoardPtr = NoteManager::getInstance()->getUserNoteBoard(userName);

            auto noteId = restinio::cast_to<UID>(params["noteId"]);
            json outputObj = *noteBoardPtr->getNote(noteId);;
            restinio::http_status_line_t status_line =
               outputObj.is_null() ? restinio::status_ok() : restinio::status_no_content();

            Utils::createGenericResponse(request->create_response(status_line))
               .append_header(restinio::http_field::content_type, "text/json; charset=utf-8")
               .set_body(outputObj.dump(3))
               .done();
         }
         catch (Utils::HttpException const& exc) {
            Utils::createErrorResponse(request, exc).done();
         }
         catch (std::exception const& exc) {
            Utils::HttpException exception(restinio::status_internal_server_error(), exc.what());
            Utils::createErrorResponse(request, exception).done();
         }
         return restinio::request_accepted();
      });
}

void
NotesEndpoint::handlePostRequests()
{
   // Create/add notes.
   _router->http_post(
      "/api/v1/:username/notes",
      [this](auto request, auto params) mutable {
         try {
            auto userName = restinio::cast_to<std::string>(params["username"]);         
            Authorization::verifyAccessToken(request->header(), userName);
            auto noteBoardPtr = NoteManager::getInstance()->getUserNoteBoard(userName);

            json noteIdArray;
            json inputArray = json::parse(request->body());
            if (inputArray.is_null()) {
               throw Utils::HttpException(restinio::status_bad_request(), "Missing request body.");
            }
            for (auto const& obj : inputArray) {
               std::shared_ptr<Note> newNote = Note::createInstance();
               std::string readInput;
               obj.at("title").get_to(readInput);
               newNote->setTitle(readInput);
               obj.at("text").get_to(readInput);
               newNote->setText(readInput);
               Color readColor;
               obj.at("color").get_to<Color>(readColor);
               // If the user has elected the right color option it will be set, otherwise we will set the color to yellow.
               // This is done in order not to stop the bulk operation in the middle of its lifetime.
               if (readColor != Color::invalid) {
                  newNote->setColor(readColor);
               }
               auto id = noteBoardPtr->createNote(newNote);
               noteIdArray.push_back(id);
            }
            json output;
            output["noteId"] = noteIdArray;
            restinio::http_status_line_t status_line =
               output["noteId"].is_null() ? restinio::status_no_content() : restinio::status_created();

            Utils::createGenericResponse(request->create_response(status_line))
               .append_header(restinio::http_field::content_type, "text/json; charset=utf-8")
               .set_body(output.dump(3))
               .done();
         }
         catch (Utils::HttpException const& exc) {
            Utils::createErrorResponse(request, exc).done();
         }
         catch (std::exception const& exc) {
            Utils::HttpException exception(restinio::status_internal_server_error(), exc.what());
            Utils::createErrorResponse(request, exception).done();
         }
         return restinio::request_accepted();
      });
}

void
NotesEndpoint::handlePutRequests()
{
   // Update notes.
   _router->http_put(
      "/api/v1/:username/notes",
      [this](auto request, auto params) mutable {         
         try {
            auto userName = restinio::cast_to<std::string>(params["username"]);
            Authorization::verifyAccessToken(request->header(), userName);
            auto noteBoardPtr = NoteManager::getInstance()->getUserNoteBoard(userName);
            
            json inputArray = json::parse(request->body());
            json badRequestArray;
            for (auto const& obj : inputArray) {
               NoteContext updateNote;
               updateNote._id = INVALID_UID;
               if (obj.contains("noteId")) {
                  updateNote._id = obj.at("noteId").get<size_t>();
               }
               if (obj.contains("title")) {
                  std::string temp;
                  obj.at("title").get_to(temp);
                  updateNote._title = std::move(temp);
               }
               if (obj.contains("text")) {
                  std::string temp;
                  obj.at("text").get_to(temp);
                  updateNote._text = std::move(temp);
               }
               if (obj.contains("color")) {
                  std::string temp;
                  obj.at("color").get_to(temp);
                  updateNote._noteColor = toColor(std::move(temp));
               }
               try {
                  noteBoardPtr->updateNote(updateNote);
               } catch (Utils::HttpException const& exc) {
                  json noteError;
                  to_json(noteError, exc);
                  noteError["noteId"] = updateNote._id;
                  badRequestArray.push_back(noteError);
               }
            }
            if (!badRequestArray.is_null()) {
               Utils::createGenericResponse(request->create_response(restinio::status_bad_request()))
                  .append_header(restinio::http_field::content_type, "text/json; charset=utf-8")
                  .set_body(badRequestArray.dump(3))
                  .done();
            }
            else {
                Utils::createNoContentResponse(request).done();
            }
         }
         catch (Utils::HttpException const& exc) {
            Utils::createErrorResponse(request, exc).done();
         }
         catch (std::exception const& exc) {
            Utils::HttpException exception(restinio::status_internal_server_error(), exc.what());
            Utils::createErrorResponse(request, exception).done();
         }

         return restinio::request_accepted();
      });
}

void
NotesEndpoint::handleDeleteRequests()
{
   _router->http_delete(
      R"(/api/v1/:username/notes/:noteId(\d+))",
      [this](auto request, auto params) mutable {
         try {
            auto userName = restinio::cast_to<std::string>(params["username"]);
            Authorization::verifyAccessToken(request->header(), userName);
            auto noteBoardPtr = NoteManager::getInstance()->getUserNoteBoard(userName);

            auto noteId = restinio::cast_to<UID>(params["noteId"]);
            auto deleted = noteBoardPtr->deleteNote(noteId);
            if (!deleted) {
               throw Utils::HttpException(restinio::status_bad_request(), "Invalid note id.");
            }
            Utils::createNoContentResponse(request).done();
         }
         catch (Utils::HttpException const& exc) {
            Utils::createErrorResponse(request, exc).done();
         }
         catch (std::exception const& exc) {
            Utils::HttpException exception(restinio::status_internal_server_error(), exc.what());
            Utils::createErrorResponse(request, exception).done();
         }
         return restinio::request_accepted();
      });
}

auto
NotesEndpoint::createNoteEndpointRequestHandler()
{
   handleGetRequests();
   handlePostRequests();
   handlePutRequests();
   handleDeleteRequests();
   return[handler = std::move(_router)](const auto& req) {
      return (*handler)(req);
   };
}

} //namspace Notes
