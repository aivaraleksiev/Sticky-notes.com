// Copyright 2021
// Author: Ayvar Aleksiev

#include "HttpException.h"

namespace Notes {
namespace Utils {

HttpException::HttpException(restinio::http_status_line_t const& httpStatusLine, std::string const& reason)
   : _httpStatus(httpStatusLine)
{
   auto const& oldReason = _httpStatus.reason_phrase();
   _httpStatus.reason_phrase(oldReason + ": " + reason);
   _reason << _httpStatus.status_code().raw_code() << ", " << _httpStatus.reason_phrase();
}

restinio::http_status_line_t
HttpException::getHttpStatusLine() const
{
   return _httpStatus;
}

char const*
HttpException::what() const
{
   return _reason.str().c_str();
}

// Friend function of HttpException.
void to_json(json& j, HttpException const& exc)
{
   j = json { {"Http error code", exc._httpStatus.status_code().raw_code()},
              {"reason", exc._httpStatus.reason_phrase()}
            };
}

// Friend function of HttpException.
void from_json(const json& j, HttpException& exc)
{
   std::uint16_t httpCode;
   std::string reason;

   j.at("HTTP error code").get_to(httpCode);
   j.at("reason").get_to(reason);

   exc._httpStatus.status_code(restinio::http_status_code_t(httpCode));
   exc._httpStatus.reason_phrase(reason);
}

} // namespace Utils
} // namespace Notes