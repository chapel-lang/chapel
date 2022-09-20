/*
 * Copyright 2021-2022 Hewlett Packard Enterprise Development LP
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
#include "chpl/framework/ErrorWriter.h"

namespace chpl {

static ssize_t posToFileIndex(const char* buf, int row, int col) {
  int curRow = 1, curCol = 1;
  size_t idx = 0;
  while (buf[idx] != '\0') {
    if (curRow == row && curCol == col) {
      return idx;
    }
    if (buf[idx] == '\n') {
      curCol = 0;
      curRow++;
    }
    curCol++;
    idx++;
  }
  if (curRow == row && curCol == col) {
    return idx;
  }
  return -1;
}

std::string ErrorWriter::fileText(const Location& loc) {
  auto path = loc.path();
  if (path.isEmpty()) return "";
  auto fileText = parsing::fileText(context, path);
  return fileText.text();
}

const char* ErrorWriter::kindText(ErrorBase::Kind kind) {
  switch (kind) {
    case ErrorBase::Kind::NOTE: return "note";
    case ErrorBase::Kind::WARNING: return "\033[33m" "warning";
    case ErrorBase::Kind::SYNTAX: return "\033[31m" "syntax";
    case ErrorBase::Kind::ERROR: return "\033[31m" "error";
  }
}

void ErrorWriter::writeErrorHeading(ErrorBase::Kind kind, Location loc) {
  auto path = loc.path().c_str();
  int lineno = loc.line();
  bool validPath = (path != nullptr && path[0] != '\0');

  lastLocation_ = loc;

  if (outputFormat_ == DETAILED) {
    // In detailed mode, print some error decoration
    oss_ << "=== ";
  }
  if (outputFormat_ != MESSAGE_ONLY) {
    // As long as we're not only printing the message, print the error location.
    oss_ << kindText(kind) << "\033[0m in ";
    if (validPath && lineno > 0) oss_ << path << ":" << lineno;
    else if (validPath) oss_ << path;
    else oss_ << "(unknown location)";
  }

  if (outputFormat_ == DETAILED) {
    // Second part of the error decoration
    oss_ << " ===" << std::endl;
  } else if (outputFormat_ != MESSAGE_ONLY) {
    // We printed location, so add a separating colon.
    oss_ << ": ";
  }
}

void ErrorWriter::writeCode(const Location& location,
                           const std::vector<Location>& toHighlight) {
  if (outputFormat_ != DETAILED || context == nullptr) return;

  auto str = fileText(location);
  if (str.empty()) return;

  size_t startIndex = posToFileIndex(str.c_str(), location.firstLine(), 1);
  size_t endIndex = posToFileIndex(str.c_str(), location.lastLine()+1, 1);

  std::vector<std::pair<size_t, size_t>> highlightRanges;
  for (auto hlLoc : toHighlight) {
    size_t hlStart = posToFileIndex(str.c_str(), hlLoc.firstLine(), hlLoc.firstColumn());
    size_t hlEnd = posToFileIndex(str.c_str(), hlLoc.lastLine(), hlLoc.lastColumn());
    highlightRanges.push_back(std::make_pair(hlStart, hlEnd));
  }

  bool needsLine = true;
  bool needsHighlight = false;
  bool neededHighlight = false;
  oss_ << std::endl;
  for (size_t i = startIndex; i < endIndex; i++) {
    needsHighlight = false;
    for (auto& range : highlightRanges) {
      if (i >= range.first && i < range.second) {
        needsHighlight = true;
        break;
      }
    }

    if (needsHighlight && !neededHighlight) {
      oss_ << "\033[4m";
    } else if (!needsHighlight && neededHighlight) {
      oss_ << "\033[24m";
    }
    neededHighlight = needsHighlight;

    if (needsLine) {
      needsLine = false;
      oss_ << "    ";
    }
    oss_ << str[i];
    needsLine = str[i] == '\n';
  }
  oss_ << "\0x33[24m" << std::endl;
}

void ErrorWriter::writeCode(const uast::AstNode* place,
                           const std::vector<const uast::AstNode*>& toHighlight) {
  writeCode<>(place, toHighlight);
}

void ErrorWriter::writeNewline() {
  oss_ << std::endl;
}

} // end namespace chpl
