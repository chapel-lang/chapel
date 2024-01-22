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

#include <string>

extern const char* yychpl_get_text(yyscan_t scanner);

namespace {
enum ErrorKind {
  EMPTY_ERROR_MESSAGE,
  SYNTAX_ERROR,
  MEMORY_EXHAUSTED,
  UNKNOWN
};
} // end anonymous namespace

static enum ErrorKind determineErrorKind(const char* msg) {
  if (0 == strlen(msg)) return EMPTY_ERROR_MESSAGE;
  if (!strcmp("syntax error", msg)) return SYNTAX_ERROR;
  if (!strcmp("memory exhausted", msg)) return MEMORY_EXHAUSTED;
  return UNKNOWN;
}

void yychpl_error(YYLTYPE*       loc,
                  ParserContext* context,
                  const char*    errorMessage) {
  auto errorKind = determineErrorKind(errorMessage);
  auto nearestToken = std::string(yychpl_get_text(context->scanner));

  switch (errorKind) {
    case EMPTY_ERROR_MESSAGE:
    case SYNTAX_ERROR:
      CHPL_PARSER_REPORT(context, BisonSyntaxError, *loc, nearestToken);
      break;
    case UNKNOWN:
      CHPL_PARSER_REPORT(context, BisonUnknownError, *loc,
                         std::string(errorMessage), nearestToken);
      break;
    case MEMORY_EXHAUSTED:
      CHPL_PARSER_REPORT(context, BisonMemoryExhausted, *loc);
      break;
  }
}

