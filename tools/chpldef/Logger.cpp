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

#include "./Logger.h"

#include "chpl/util/filesystem.h"
#include "chpl/util/printf.h"

#include "llvm/Support/FileSystem.h"
#include "llvm/Support/JSON.h"

// Wrapper to forward printf arguments to logger.
#define VPRINTF_FORWARD_(fmt__, func__) \
  do { \
    va_list args__; \
    va_start(args__, fmt__); \
    func__(fmt__, args__); \
    va_end(args__); \
  } while (0)

namespace chpldef {

// TODO: Verify file exists.
Logger::Logger(Output output, std::string filePath)
    : output_(output), filePath_(std::move(filePath)) {
  if (output == Logger::STDERR) filePath_ = "<stderr>";
  if (output == Logger::STDOUT) filePath_ = "<stdout>";
  if (!start()) CHPL_ASSERT(!isLoggingToBuiltin());
}

Logger::Logger(Logger&& other) {
  *this = std::move(other);
}

Logger::Logger() : Logger(Logger::STDERR, "<stderr>") {}

Logger Logger::createForStderr() {
  return Logger();
}

Logger Logger::createForStdout() {
  return Logger(Logger::STDOUT, "<stdout>");
}

Logger Logger::createForFile(std::string filePath) {
  return Logger(Logger::FILEPATH, filePath);
}

Logger& Logger::operator=(Logger&& other) noexcept {
  output_ = other.output_;
  filePath_ = other.filePath_;
  flushImmediately_ = other.flushImmediately_;
  level_ = other.level_;
  header_ = other.header_;
  stream_.swap(other.stream_);
  if (other.stream_.is_open()) other.stream_.close();
  return *this;
}

Logger::~Logger() {
  if (isLogging()) stream_.close();
}

bool Logger::isLoggingToBuiltin() const {
  return output_ != Logger::FILEPATH;
}

const char* Logger::levelToString(Level level) {
  switch (level) {
    case Logger::OFF: return "off";
    case Logger::MESSAGES: return "messages";
    case Logger::VERBOSE: return "verbose";
    case Logger::TRACE: return "trace";
  }
  CHPLDEF_IMPOSSIBLE();
  return nullptr;
}

bool Logger::isLogging() const {
  if (isLoggingToBuiltin()) return true;
  return !filePath_.empty() && stream_.is_open();
}

void Logger::setFlushImmediately(bool flushImmediately) {
  this->flushImmediately_ = flushImmediately;
}

bool Logger::start() {
  if (isLogging()) return true;
  CHPL_ASSERT(!isLoggingToBuiltin());
  stream_.open(filePath_);
  bool ret = stream_.is_open() && !stream_.bad() && !stream_.fail();
  if (!ret) stream_.close();
  return ret;
}

void Logger::stop() {
  if (!isLogging() || isLoggingToBuiltin()) return;
  if (stream_.is_open()) stream_.close();
}

void Logger::flush() {
  if (!isLogging()) return;
  switch (output_) {
    case STDERR: std::cerr.flush();
    case STDOUT: std::cout.flush();
    case FILEPATH: stream_.flush();
  }
}

void Logger::logit(const char* msg) {
  if (!isLogging()) return;
  auto& out = stream();
  if (!header_.empty()) out << header_ << " ";
  out << msg;
  if (flushImmediately_) flush();
}

void Logger::logit(std::string msg) {
  this->logit(msg.c_str());
}

static bool canLog(Logger::Level have, Logger::Level want) {
  switch (have) {
    case Logger::OFF: return false;
    case Logger::MESSAGES: return want == Logger::MESSAGES;
    case Logger::VERBOSE: return want == Logger::MESSAGES ||
                                 want == Logger::VERBOSE;
    case Logger::TRACE: return want != Logger::OFF;
  }
  CHPLDEF_IMPOSSIBLE();
  return false;
}

void Logger::log(Level level, const char* fmt, ...) {
  va_list args;
  va_start(args, fmt);
  vlog(level, fmt, args);
  va_end(args);
}

void Logger::message(const char* fmt, ...) {
  VPRINTF_FORWARD_(fmt, this->vmessage);
}

void Logger::verbose(const char* fmt, ...) {
  VPRINTF_FORWARD_(fmt, this->vverbose);
}

void Logger::trace(const char* fmt, ...) {
  VPRINTF_FORWARD_(fmt, this->vtrace);
}

void Logger::vlog(Level level, const char* fmt, va_list args) {
  if (!canLog(level_, level)) return;
  auto str = chpl::vprintToString(fmt, args);
  this->logit(str.c_str());
}

void Logger::vmessage(const char* fmt, va_list args) {
  vlog(Logger::MESSAGES, fmt, args);
}

void Logger::vverbose(const char* fmt, va_list args) {
  vlog(Logger::VERBOSE, fmt, args);
}

void Logger::vtrace(const char* fmt, va_list args) {
  vlog(Logger::TRACE, fmt, args);
}

} // end namespace 'chpldef'
