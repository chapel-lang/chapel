/*
 * Copyright 2020-2024 Hewlett Packard Enterprise Development LP
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

#include "misc.h"
#include <cstdint>
#include <string>

namespace chpldef {

class Server;

/** This is the base class for various transport layers. */
class Transport {
public:

  /** Error codes indicate the result of a read or write. */
  enum Status {
    OK = 0,
    ERROR = 1,
    ERROR_JSON_PARSE_FAILED = 2,
    ERROR_INVALID_SIZE      = 4,
  };

  /** Provides a way to configure each read or write. */
  enum BehaviorTag {
    DEFAULT             = 0,
    READ_UNTIL_NEWLINE  = 1
  };

  /** Behavior is a bitfield composed of 'BehaviorTag'. */
  using Behavior = int;

  virtual ~Transport() = default;

  /** Read up to 'size' bytes into 'str' with the given behaviors 'b'. */
  virtual Status read(int64_t size, std::string& str, Behavior b=0) = 0;

  /** Send the bytes of 'str' with the given behaviors 'b'. */
  virtual Status send(const std::string& str, Behavior b=0) = 0;

  /** Read JSON into the given JSON value 'j'. */
  Status readJson(Server* ctx, JsonValue& j);

  /** Serialize the contents of the JSON 'j' into text and send it. */
  Status sendJson(Server* ctx, const JsonValue& j);
};

/** This transport layer performs blocking reads over STDIN and STDOUT. */
class TransportStdio final : public Transport {
public:
  TransportStdio() {}
  virtual ~TransportStdio() = default;
  virtual Status read(int64_t size, std::string& str,
                      Behavior b=0) override;
  virtual Status send(const std::string& str,
                      Behavior b=0) override;
};

} // end namespace 'chpldef'

#endif
