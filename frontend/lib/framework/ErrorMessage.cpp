/*
 * Copyright 2021-2025 Hewlett Packard Enterprise Development LP
 * Other additional copyright holders may be indicated within.
 *
 * The entirety of this work is licensed under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except
 * in compliance with the License.
 *
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <cstdarg>
#include <cstdlib>

#include "chpl/framework/ErrorMessage.h"
#include "chpl/parsing/parsing-queries.h"
#include "chpl/uast/AstNode.h"

namespace chpl {

Location IdOrLocation::computeLocation(Context* context) const {
  // if the ID is set, determine the location from that
  if (!id_.isEmpty()) {
    Location loc = parsing::locateId(context, id_);
    return loc;
  }

  // otherwise, use the location stored here
  return location_;
}

IdOrLocation IdOrLocation::createForCommandLineLocation(Context* context) {
  auto name = UniqueString::get(context, "<command line>");
  return IdOrLocation(Location(name));
}

ErrorMessage::ErrorMessage(Kind kind, IdOrLocation idOrLoc, std::string message)
    : IdOrLocation(std::move(idOrLoc)), kind_(kind), message_(std::move(message)) {

}

void ErrorMessage::addDetail(ErrorMessage err) {
  details_.push_back(std::move(err));
}

} // namespace chpl
