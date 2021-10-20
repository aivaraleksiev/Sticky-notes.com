// Copyright 2021
// Author: Ayvar Aleksiev

#include "Utils.h"


namespace {
   std::atomic<size_t> sUid { 0 };
} // anonymous namepspace 

namespace Notes {
namespace Utils {

UID
UIDGenerator::generateUID()
{
   return ++sUid;
}

} // namespace Note
} // namespace Utils