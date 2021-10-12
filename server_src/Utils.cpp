// Copyright 2021
// Author: Ayvar Aleksiev

#include "Utils.h"


namespace {
 std::atomic<size_t> uid { 0 };
}

namespace Notes {
namespace Utils {

size_t
UIDGenerator::generateUID()
{
   return ++uid;
}

} // namespace Note
} // namespace Utils