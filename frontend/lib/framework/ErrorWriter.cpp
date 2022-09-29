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

void ErrorWriterBase::writeHeading(ErrorBase::Kind kind, const ID& id, const std::string& str) {
  writeHeading(kind, errordetail::locate(context, id), str);
}

void ErrorWriterBase::writeHeading(ErrorBase::Kind kind,
                                    const uast::AstNode* node,
                                    const std::string& str) {
  writeHeading(kind, node->id(), str);
}

void ErrorWriterBase::writeNote(const ID& id, const std::string& str) {
  writeNote(errordetail::locate(context, id), str);
}

void ErrorWriterBase::writeNote(const uast::AstNode* ast, const std::string& str) {
  writeNote(ast->id(), str);
}

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
  if ((curRow == row && curCol == col) || (curRow + 1 == row && col == 1)) {
    // Might not end in a newline. In that case, pretend there is one.
    return idx;
  }
  return -1;
}

static std::string fileText(Context* context, const Location& loc) {
  auto path = loc.path();
  if (path.isEmpty()) return "";
  auto fileText = parsing::fileText(context, path);
  return fileText.text();
}

void ErrorWriter::setColor(TermColorName color) {
  if (useColor_) {
    oss_ << getColorFormat(color);
  }
}

static const char* kindText(ErrorBase::Kind kind) {
  switch (kind) {
    case ErrorBase::Kind::NOTE: return "note";
    case ErrorBase::Kind::WARNING: return "warning";
    case ErrorBase::Kind::SYNTAX: return "syntax";
    case ErrorBase::Kind::ERROR: return "error";
  }
  return "(unknown kind)";
}

static TermColorName kindColor(ErrorBase::Kind kind) {
  switch (kind) {
    case ErrorBase::Kind::NOTE: return CLEAR;
    case ErrorBase::Kind::WARNING: return YELLOW;
    case ErrorBase::Kind::SYNTAX: return RED;
    case ErrorBase::Kind::ERROR: return RED;
  }
  return CLEAR;
}

static void writeFile(std::ostream& oss, const Location& loc) {
  auto pathUstr = loc.path();
  auto path = pathUstr.c_str();
  int lineno = loc.line();
  bool validPath = (path != nullptr && path[0] != '\0');

  if (validPath && lineno > 0) oss << path << ":" << lineno;
  else if (validPath) oss << path;
  else oss << "(unknown location)";
}

void ErrorWriter::writeHeading(ErrorBase::Kind kind, Location loc, const std::string& str) {
  if (outputFormat_ == DETAILED) {
    // In detailed mode, print some error decoration
    oss_ << "--- ";
  }

  setColor(kindColor(kind));
  oss_ << kindText(kind);
  setColor(CLEAR);
  oss_ << " in ";
  writeFile(oss_, loc);
  if (outputFormat_ == DETAILED) {
    // Second part of the error decoration
    oss_ << " ---" << std::endl;
  } else {
    // We printed location, so add a separating colon.
    oss_ << ": ";
  }
  oss_ << str << std::endl;
}

void ErrorWriter::writeNote(Location loc, const std::string& str) {
  if (outputFormat_ == BRIEF) {
    // Indent notes in brief mode to make things easier to organize
    oss_ << "  note in ";
    writeFile(oss_, loc);
    oss_ << ": ";
  }
  oss_ << str << std::endl;
}

static void printBlank(std::ostream& os, int n) {
  for (int i = 0; i < n; i++) {
    os << ' ';
  }
}

static void printLineNo(std::ostream& os, size_t gutterLength, int line) {
  std::string lineStr = std::to_string(line);
  printBlank(os, gutterLength - lineStr.size());
  os << "  " << lineStr << " | ";
}

void ErrorWriter::writeCode(const Location& location,
                           const std::vector<Location>& toHighlight) {
  if (outputFormat_ != DETAILED || context == nullptr) return;

  auto str = fileText(context, location);
  if (str.empty()) return;

  ssize_t startIndex = posToFileIndex(str.c_str(), location.firstLine(), 1);
  ssize_t endIndex = posToFileIndex(str.c_str(), location.lastLine()+1, 1);

  std::vector<std::pair<ssize_t, ssize_t>> ranges;
  for (auto hlLoc : toHighlight) {
    ssize_t hlStart = posToFileIndex(str.c_str(), hlLoc.firstLine(), hlLoc.firstColumn());
    ssize_t hlEnd = posToFileIndex(str.c_str(), hlLoc.lastLine(), hlLoc.lastColumn());
    ranges.push_back(std::make_pair(hlStart, hlEnd));
  }

  size_t gutterSize = std::to_string(location.lastLine()).size();
  int lineNumber = location.firstLine();

  // printBlank(lineLength + 2);
  // oss_ << "(file " << location.path() << ")" << std::endl;
  printBlank(oss_, gutterSize + 3);
  oss_ << "|";
  oss_ << std::endl;
  std::string highlightString = "";
  bool printHighlight = false;
  bool needsLine = true;
  for (ssize_t i = startIndex; i < endIndex; i++) {
    if (needsLine) {
      printLineNo(oss_, gutterSize, lineNumber);
      needsLine = false;
    }
    bool highlight = std::any_of(ranges.begin(), ranges.end(), [&](auto range) {
      return i >= range.first && i < range.second;
    });
    highlightString += highlight ? '^' : ' ';
    printHighlight = printHighlight || highlight;

    oss_ << str[i];
    if (str[i] == '\n') {
      if (printHighlight) {
        printBlank(oss_, gutterSize + 3);
        oss_ << "| " << highlightString << std::endl;
      }
      lineNumber += 1;
      needsLine = true;
      printHighlight = false;
      highlightString = "";
    }
  }
  if (!needsLine) {
    // No newline at the end in the file, insert one ourselves.
    oss_ << std::endl;
  }
  printBlank(oss_, gutterSize + 3);
  oss_ << "|";
  oss_ << std::endl;
}

void ErrorWriter::writeNewline() {
  oss_ << std::endl;
}

} // end namespace chpl
