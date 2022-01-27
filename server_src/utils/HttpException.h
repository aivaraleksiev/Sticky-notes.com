// Copyright 2021
// Author: Ayvar Aleksiev

#pragma once

#include <nlohmann/json.hpp>
#include <restinio/all.hpp>

using nlohmann::json;

namespace Notes {
namespace Utils {
 
class HttpException : public std::exception
{
public:
   HttpException(restinio::http_status_line_t const& httpStatusLine, std::string const& reason);

   restinio::http_status_line_t getHttpStatusLine() const;
   char const* what() const override;

   friend void to_json(nlohmann::json& j, HttpException const& exc);
   friend void from_json(nlohmann::json const& j, HttpException& exc);

private:
   restinio::http_status_line_t _httpStatus;
   std::ostringstream _reason;
};

} // namespace Utils
} // namespace Notes