/*
 * Copyright 2020-2023 Hewlett Packard Enterprise Development LP
 * Copyright 2004-2019 Cray Inc.
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

#ifndef CHPL_TOOLS_CHPLDEF_TRANSPORT_H
#define CHPL_TOOLS_CHPLDEF_TRANSPORT_H

#include "./Message.h"
#include "./Server.h"

#include <iostream>
#include <fstream>

namespace chpldef {

/**
  This class is intended to model or wrap some existing transport layer,
  but is currently empty.
*/
class Transport {
public:

  /** Helper which reads a message from a stream in a blocking way.
      Returns 'true' if there was an error. If there was not an
      error, then the formal 'outJson' will contain the JSON for the
      message. */
  static bool
  readJsonBlocking(chpldef::Server* ctx, std::istream& is,
                   JsonValue& out);

  /** Helper which sends JSON to a stream in a blocking way.
      Returns 'true' if there was an error. */
  static bool
  sendJsonBlocking(chpldef::Server* ctx, std::ostream& os,
                   const JsonValue& json);
};

} // end namespace 'chpldef'

#endif
