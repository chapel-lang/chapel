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

#include "Transport.h"
#include "Server.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/JSON.h"
#include <cinttypes>
#include <iostream>
#include <fstream>

namespace chpldef {

static Transport::Status
streamReadLineTrimCarriageReturn(Transport* tp, std::string& out) {
  auto ret = tp->read(0, out, Transport::READ_UNTIL_NEWLINE);
  if (!out.empty() && out.back() == '\n') out.pop_back();
  if (!out.empty() && out.back() == '\r') out.pop_back();
  return ret;
}

// SEE: 'clangd::JSONTransport::readStandardMessage' for signal handling.
static bool
streamReadLength(std::istream& is, int64_t length, std::string& out) {
  out.resize(length);
  for (int64_t bytes = length, read; bytes > 0; bytes -= read) {
    is.read(&out[0], bytes);
    read = is.gcount();
  }
  return true;
}

// TODO: Remove the asserts by logging and returning an error/message.
// SEE: 'clangd::JSONTransport::readStandardMessage' for robust impl...
Transport::Status Transport::readJson(Server* ctx, JsonValue& j) {
  Status status = Transport::OK;
  bool err = false;
  std::string line;
  int64_t length = 0;

  // Note the fatal crashes in this function. I've inserted these because
  // if we fail to read a line in the protocol, it's difficult or
  // impossible for the server to recover. At any rate, we can worry about
  // recovering later (and ideally, not reinvent the wheel).
  status = streamReadLineTrimCarriageReturn(this, line);
  if (status) CHPLDEF_FATAL(ctx, "Reading first line of message data");

  static constexpr const char* prefix = "Content-Length:";
  err |= !(line.rfind(prefix) == 0);
  if (err) CHPLDEF_FATAL(ctx, "Expected message prefix %s", prefix);

  auto str = line.substr(line.find(" "), std::string::npos);
  err |= str.empty();
  if (err) CHPLDEF_FATAL(ctx, "Expected length following %s", prefix);

  err |= !chpldef::cast(str, length);
  err |= length <= 0;
  if (err) CHPLDEF_FATAL(ctx, "Message length %s is invalid", str.c_str());

  status = streamReadLineTrimCarriageReturn(this, line);
  if (status) CHPLDEF_FATAL(ctx, "Reading CRLF before payload");

  err |= !line.empty();
  if (err) CHPLDEF_FATAL(ctx, "Expected CRLF before payload");

  status = this->read(length, line);
  if (status) CHPLDEF_FATAL(ctx, "Failed to read payload data");

  // Parse and write out the JSON now.
  if (auto json = llvm::json::parse(line)) {
    j = std::move(*json);
    return OK;
  }

  // Failed to parse.
  ctx->verbose("Failed to parse JSON object of size: %" PRId64 "\n", length);
  ctx->trace("String is: %s\n", line.c_str());
  return ERROR_JSON_PARSE_FAILED;
}

// TODO: Is there a way to write the contents without the copies?
Transport::Status Transport::sendJson(Server* ctx, const JsonValue& j) {
  std::stringstream ss;
  const bool pretty = false;
  std::string contents = jsonToString(j, pretty);

  ss << "Content-Length: " << contents.size() << "\r\n\r\n" << contents;

  auto ret = this->send(ss.str());

  if (ret != OK) {
    ctx->trace("Failed to send JSON object of length: %zu\n",
               contents.size());
    ctx->trace("Payload is: %s\n", contents.c_str());
  }

  return ret;
}

Transport::Status
TransportStdio::read(int64_t size, std::string& str, Transport::Behavior b) {
  if (size < 0) return ERROR_INVALID_SIZE;

  bool readEntireContents = (size == 0);
  bool readUntilNewline = (b & READ_UNTIL_NEWLINE);
  int64_t bytesToRead = size;
  Status ret = OK;

  if (readUntilNewline) {
    if (!readEntireContents) CHPLDEF_TODO();
    std::getline(std::cin, str, '\n');
    bool err = std::cin.fail() || std::cin.bad();
    if (err) ret = ERROR;
    return ret;
  }

  if (readEntireContents) CHPLDEF_TODO();
  bool ok = streamReadLength(std::cin, bytesToRead, str);
  if (!ok) ret = ERROR;

  return ret;
}

Transport::Status
TransportStdio::send(const std::string& str, Transport::Behavior b) {
  std::cout << str;
  bool err = std::cout.fail() || std::cout.bad();
  return err ? ERROR : OK;
}

} // end namespace 'chpldef'
