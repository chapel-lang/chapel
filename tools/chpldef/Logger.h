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

#ifndef CHPL_TOOLS_CHPLDEF_LOGGER_H
#define CHPL_TOOLS_CHPLDEF_LOGGER_H

#include "misc.h"
#include "llvm/Support/JSON.h"
#include <chrono>
#include <fstream>
#include <iostream>
#include <thread>

namespace chpldef {

class Logger {
public:
  /** These correspond to LSP notification levels, except for 'TRACE'. */
  enum Level { OFF, MESSAGES, VERBOSE, TRACE };

  /** TODO: Should probably avoid logging to STDOUT for now. */
  enum Output { STDERR, STDOUT, FILEPATH };

private:
  Output output_ = STDERR;
  std::string filePath_;
  std::ofstream stream_;
  bool flushImmediately_ = false;
  Level level_ = MESSAGES;
  std::string header_;

  Logger(Output output, std::string filePath);

public:
  Logger();
  Logger(Logger&& other);
 ~Logger();
  static Logger createForStderr();
  static Logger createForStdout();
  static Logger createForFile(std::string filePath);

  Logger& operator=(Logger&& other) noexcept;

  inline const std::string& filePath() const { return filePath_; }
  bool isLoggingToBuiltin() const;
  bool isLogging() const;
  inline Level level() const { return level_; }
  static const char* levelToString(Level level);
  inline const char* levelToString() const { return levelToString(level_); }
  inline Output output() const { return output_; }
  inline void setLevel(Level level) { this->level_ = level; }
  void setFlushImmediately(bool flushImmediately);
  inline bool flushImmediately() const { return flushImmediately_; }
  void setHeader(std::string header) { header_ = std::move(header); }
  inline const std::string& header() const { return header_; }

  /** Get a mutable handle to the underlying output stream. */
  inline auto& stream() {
    return output_ == STDERR ? std::cerr : output_ == STDOUT ? std::cout
                             : stream_;
  }

  /** Returns 'true' if the log was started. */
  bool start();
  void stop();
  void flush();
  void logit(const char* msg);
  void logit(std::string msg);

  CHPLDEF_PFMT(3, 4, void log(Level level, const char* fmt, ...));
  CHPLDEF_PFMT(2, 3, void message(const char* fmt, ...));
  CHPLDEF_PFMT(2, 3, void verbose(const char* fmt, ...));
  CHPLDEF_PFMT(2, 3, void trace(const char* fmt, ...));
  void vlog(Level level, const char* fmt, va_list args);
  void vmessage(const char* fmt, va_list args);
  void vverbose(const char* fmt, va_list args);
  void vtrace(const char* fmt, va_list args);
};

} // end namespace 'chpldef'

#endif
