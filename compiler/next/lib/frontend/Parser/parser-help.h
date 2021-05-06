/*
 * Copyright 2020-2021 Hewlett Packard Enterprise Development LP
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

void yyerror(YYLTYPE*       loc,
             ParserContext* context,
             const char*    errorMessage) {
  context->errors.push_back(ParserError(*loc, errorMessage));
}

void noteError(YYLTYPE location, ParserContext* context, const char* s) {
  context->errors.push_back(ParserError(location, s));
}
void noteError(YYLTYPE location, ParserContext* context, const std::string s) {
  context->errors.push_back(ParserError(location, s));
}

static ErroneousExpression* raiseError(YYLTYPE location,
                                       ParserContext* context,
                                       const char* errorMessage) {
  // note the error for printing
  yyerror(&location, context, errorMessage);
  Location ll = context->convertLocation(location);
  // return an error sentinel
  return ErroneousExpression::build(context->builder, ll).release();
}

// these helpers can be used in the semantic actions
#define BUILDER (context->builder)
#define STMT(LOC,AST) makeCommentsAndStmt(context->gatherComments(LOC), AST)
#define ENDSTMT() context->clearComments();

#define STR(s) PODUniqueString::build(context->context(), s)
#define LOC(loc) context->convertLocation(loc)

// ERROR creates an error and returns an error sentinel Expression
#define ERROR(LOC,MSG) raiseError(LOC, context, MSG)

#define TODOEXPR(LOC) raiseError(LOC, context, "not implemented yet")
#define TODOSTMT(LOC) makeCommentsAndStmt(context->gatherComments(LOC), raiseError(LOC, context, "not implemented yet"))
#define TODOLIST(LOC) context->makeList(raiseError(LOC, context, "not implemented yet"))

