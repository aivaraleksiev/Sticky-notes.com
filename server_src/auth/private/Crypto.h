// Copyright 2021
// Author: Ayvar Aleksiev

#pragma once

namespace Notes {
namespace Utils {

#include <string>
// Generate password hash using PBKDF2 and sha 256.
std::string generatePasswordHash(std::string const& password);

} // namespace Note
} // namespace Utils