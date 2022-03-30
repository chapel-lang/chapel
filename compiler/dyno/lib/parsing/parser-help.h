/*
 * Copyright 2020-2022 Hewlett Packard Enterprise Development LP
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

void yychpl_error(YYLTYPE*       loc,
                  ParserContext* context,
                  const char*    errorMessage) {
  std::string msg;
  const char* tokenText = yychpl_get_text(context->scanner);
  if (strlen(tokenText) > 0) {
    msg += "near '";
    msg += tokenText;
    msg += "'";
  } else {
    // Not very helpful, but default parser errors aren't...
    assert(msg.size() == 0);
  }

  auto err = ParserError(*loc, msg, ErrorMessage::SYNTAX);
  context->noteSyntaxError(std::move(err));
}

// these helpers can be used in the semantic actions
#define BUILDER (context->builder)
#define STMT(LOC,AST) makeCommentsAndStmt(context->gatherComments(LOC), AST)
#define ENDSTMT() context->clearComments();

#define STR(s) PODUniqueString::get(context->context(), s)
#define LOC(loc) context->convertLocation(loc)
#define LOC2(STARTLOC, ENDLOC) \
  context->convertLocation(context->makeSpannedLocation(STARTLOC, ENDLOC))

// ERROR creates an error and returns an error sentinel AstNode
#define ERROR(LOC,MSG) context->raiseError(LOC, MSG)

#define TODOEXPR(LOC) context->raiseError(LOC, "not implemented yet")
#define TODOSTMT(LOC) makeCommentsAndStmt(context->gatherComments(LOC), context->raiseError(LOC, "not implemented yet"))
#define TODOLIST(LOC) context->makeList(context->raiseError(LOC, "not implemented yet"))

