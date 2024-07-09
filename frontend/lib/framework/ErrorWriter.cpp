/*
 * Copyright 2021-2024 Hewlett Packard Enterprise Development LP
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

static bool hiddenPunctuationInBrief(char c) {
  return c == ':' || c == '.';
}

void ErrorWriterBase::tweakErrorString(std::string& str) const {
  if (outputFormat_ == ErrorWriter::DETAILED && !str.empty()) {
    // Capitalize errors in detailed mode
    str[0] = std::toupper(str[0]);
  }

  if (outputFormat_ == ErrorWriter::BRIEF) {
    // Strip punctuation at the end of the error in brief mode
    while (!str.empty() &&
        hiddenPunctuationInBrief(str.back())) {
      str.pop_back();
    }
  }
}

void ErrorWriterBase::writeHeading(ErrorBase::Kind kind,
                                   ErrorType type,
                                   const uast::AstNode* node,
                                   const std::string& str) {
  writeHeading(kind, type, node->id(), str);
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
  if (!parsing::hasFileText(context, std::string(path.c_str()))) return "";
  auto fileText = parsing::fileText(context, path);
  return fileText.text();
}

bool ErrorWriter::noteFilePath(std::string newPath) {
  bool toReturn = lastFilePath_ != newPath;
  lastFilePath_ = std::move(newPath);
  return toReturn;
}

void ErrorWriter::setColor(TermColorName color) {
  if (useColor_) {
    oss_ << getColorFormat(color);
  }
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

static std::string locToPath(Context* context, const Location& loc) {
  UniqueString pathUstr = loc.path();
  if (context) pathUstr = context->adjustPathForErrorMsg(pathUstr);
  auto path = pathUstr.c_str();
  bool validPath = (path != nullptr && path[0] != '\0');
  if (validPath) return path;
  return "";
}

static void writeFile(Context* context,
                      std::ostream& oss,
                      const Location& loc,
                      std::string* outFilePath = nullptr) {
  int lineno = loc.line();
  auto path = locToPath(context, loc);
  if (outFilePath) *outFilePath = path;

  if (!path.empty()) {
    if (lineno > 0) oss << path << ":" << lineno;
    else oss << path;
  } else {
    oss << "(unknown location)";
  }
}

void ErrorWriter::writeHeading(ErrorBase::Kind kind, ErrorType type,
                               IdOrLocation loc, const std::string& str) {
  if (outputFormat_ == DETAILED) {
    // In detailed mode, print some error decoration
    oss_ << "─── ";
  }

  setColor(kindColor(kind));
  oss_ << ErrorBase::getKindName(kind);
  setColor(CLEAR);
  oss_ << " in ";

  // Printing the header prints the file path, so we need to update the
  // 'lastFilePath_' field.
  std::string printedPath;
  writeFile(context_, oss_, errordetail::locate(context_, loc), &printedPath);
  noteFilePath(std::move(printedPath));

  if (outputFormat_ == DETAILED) {
    // Second part of the error decoration
    const char* name = ErrorBase::getTypeName(type);
    if (name != nullptr) {
      oss_ << " [" << name << "]";
    }
    oss_ << " ───" << std::endl;
    // In detailed mode, the body is indented.
    oss_  << "  ";
  } else {
    // We printed location, so add a separating colon.
    oss_ << ": ";
  }

  oss_ << str << std::endl;
}

void ErrorWriter::writeNote(IdOrLocation loc, const std::string& str) {
  if (outputFormat_ == BRIEF) {
    // Indent notes in brief mode to make things easier to organize
    oss_ << "  note in ";
    writeFile(context_, oss_, errordetail::locate(context_, loc));
    oss_ << ": ";
  } else {
    // In detailed mode, the body is indented.
    oss_ << "  ";
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
  if (outputFormat_ != DETAILED || context_ == nullptr) return;

  auto str = fileText(context_, location);
  if (str.empty()) return;

  ssize_t startIndex = posToFileIndex(str.c_str(), location.firstLine(), 1);
  ssize_t endIndex = posToFileIndex(str.c_str(), location.lastLine()+1, 1);

  std::vector<std::pair<ssize_t, ssize_t>> ranges;
  for (auto hlLoc : toHighlight) {
    ssize_t hlStart = posToFileIndex(str.c_str(), hlLoc.firstLine(), hlLoc.firstColumn());
    ssize_t hlEnd = posToFileIndex(str.c_str(), hlLoc.lastLine(), hlLoc.lastColumn());
    ranges.push_back(std::make_pair(hlStart, hlEnd));
  }

  // Example:
  //   Error message text
  //         |
  //     123 | proc f() {}
  //         |
  //
  // ^^   Error message indent
  //   ^^ Code indent
  //     ^^^ Last line length
  //        ^ One extra space of padding

  // Gutter size includes just the code indent and the last line length.
  size_t gutterSize = std::to_string(location.lastLine()).size() + 2;
  // When not printing the line number, we need to manually print the error
  // message indent and the one extra space of padding.
  size_t codeIndent = gutterSize+3;
  int lineNumber = location.firstLine();

  // Print the file path if it's changed since the last code block. Printing
  // a code block will display the file path if needed, so lastFilePath_ needs
  // to be updated.
  if (noteFilePath(locToPath(context_, location))) {
    printBlank(oss_, codeIndent - 1);
    oss_ << "--> " << lastFilePath_ << std::endl;
  }

  printBlank(oss_, codeIndent);
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
    highlightString += highlight ? "⎺" : " ";
    printHighlight = printHighlight || highlight;

    oss_ << str[i];
    if (str[i] == '\n') {
      if (printHighlight) {
        printBlank(oss_, codeIndent);

        // Clean up trailing whitespace
        while (!highlightString.empty() && highlightString.back() == ' ') {
          highlightString.pop_back();
        }

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
  printBlank(oss_, codeIndent);
  oss_ << "|";
  oss_ << std::endl;
}

void ErrorWriter::writeNewline() {
  oss_ << std::endl;
}

} // end namespace chpl
