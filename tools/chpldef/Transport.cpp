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

#include "./Transport.h"

#include "llvm/Support/FileSystem.h"
#include "llvm/Support/JSON.h"
#include <iostream>
#include <fstream>

namespace chpldef {

// TODO: Will block until it sees a '\n' delimited line.
static bool readLineTrimCarriageReturn(std::istream& is, std::string& out) {
  std::getline(is, out);
  if (!out.empty() && out.back() == '\r') out.pop_back();
  bool ret = !is.bad() && !is.fail();
  return ret;
}

// SEE: 'clangd::JSONTransport::readStandardMessage' for signal handling.
static bool readLength(std::istream& is, int length, std::string& out) {
  out.resize(length);
  for (int bytes = length, read; bytes > 0; bytes -= read) {
    is.read(&out[0], bytes);
    read = is.gcount();
  }
  return true;
}

// TODO: Remove the asserts by logging and returning an error/message.
// SEE: 'clangd::JSONTransport::readStandardMessage' for robust impl...
bool Transport::readJsonBlocking(Server* ctx, std::istream& is,
                                 JsonValue& out) {
  std::string line;
  bool err = false;
  int length = 0;

  // Note the fatal crashes in this function. I've inserted these because
  // if we fail to read a line in the protocol, it's difficult or
  // impossible for the server to recover. At any rate, we can worry about
  // recovering later (and ideally, not reinvent the wheel).
  err |= !readLineTrimCarriageReturn(is, line);
  if (err) CHPLDEF_FATAL(ctx, "Reading first line of message data");

  static constexpr const char* prefix = "Content-Length:";
  err |= !(line.rfind(prefix) == 0);
  if (err) CHPLDEF_FATAL(ctx, "Expected message prefix %s", prefix);

  auto str = line.substr(line.find(" "), std::string::npos);
  err |= str.empty();
  if (err) CHPLDEF_FATAL(ctx, "Expected length following %s", prefix);

  err |= !chpldef::cast(str, length);
  err |= length <= 0;
  if (err) CHPLDEF_FATAL(ctx, "Message length %s is invalid", str.c_str());

  err |= !readLineTrimCarriageReturn(is, line);
  err |= !line.empty();
  if (err) CHPLDEF_FATAL(ctx, "Expected CRLF before payload");

  err |= !readLength(is, length, line);
  if (err) CHPLDEF_FATAL(ctx, "Failed to read payload data");

  // Create and write out the JSON now. No need to move the string into
  // the parse call because the JSON value will always copy.
  if (auto json = llvm::json::parse(line)) {
    out = std::move(*json);
    return true;
  }

  // Failed to parse.
  ctx->verbose("Failed to parse JSON object of length: %d\n", length);
  ctx->trace("String is: %s\n", line.c_str());
  return false;
}

// TODO: Is there a way to write to a 'std::ostream' with LLVM?
bool Transport::sendJsonBlocking(Server* ctx, std::ostream& os,
                                 const JsonValue& json) {
  const bool pretty = false;
  std::string s = jsonToString(json, pretty);
  os << "Content-Length: " << s.size() << "\r\n\r\n" << s;
  os.flush();
  bool ret = !os.bad() && !os.fail();
  return ret;
}

} // end namespace 'chpldef'
