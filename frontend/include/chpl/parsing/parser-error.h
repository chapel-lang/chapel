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

#ifndef CHPL_PARSING_PARSER_ERROR_H
#define CHPL_PARSING_PARSER_ERROR_H

/**
  Helper macro to report errors from the parser, which takes care of saving the
  error in the ParserContext, retrieving the global Context from the
  ParserContext, and converting YYLTYPE locations.

  Evaluates to an ErroneousExpression error sentinel at the location of the
  error, which may be used or ignored.
 */
#define CHPL_PARSER_REPORT(P_CONTEXT__, NAME__, LOC__, EINFO__...)         \
  (P_CONTEXT__->saveError(CHPL_REPORT(P_CONTEXT__->context(), NAME__,      \
                                      P_CONTEXT__->convertLocation(LOC__), \
                                      ##EINFO__)),                         \
   ErroneousExpression::build(P_CONTEXT__->builder,                        \
                              P_CONTEXT__->convertLocation(LOC__))         \
       .release())

// Simplified versions of CHPL_PARSER_REPORT which report an error/syntax error
// message without a specialized error class or additional info
#define CHPL_PARSER_REPORT_ERR(P_CONTEXT__, LOC__, MSG__) \
  CHPL_PARSER_REPORT(P_CONTEXT__, ParseErr, LOC__, MSG__)
#define CHPL_PARSER_REPORT_SYNTAX(P_CONTEXT__, LOC__, MSG__) \
  CHPL_PARSER_REPORT(P_CONTEXT__, ParseSyntax, LOC__, MSG__)

/**
  Helper macros to report errors from the lexer, including retrieving the
  global Context and lexer-specific location adjustments.

  Errors reported via this are forwarded on to parser error reporting.
 */
#define CHPL_LEXER_REPORT(SCANNER__, NLINES__, NCOLS__, NAME__, EINFO__...) \
  CHPL_LEXER_REPORT_ACTUAL(SCANNER__, NLINES__, NCOLS__,                    \
                           /* MOVE_TO_END__ */ false, NAME__, ##EINFO__)

#define CHPL_LEXER_REPORT_SYNTAX(SCANNER__, NLINES__, NCOLS__, MSG__) \
  CHPL_LEXER_REPORT(SCANNER__, NLINES__, NCOLS__, ParseSyntax, MSG__)

// this variant moves the beginning of the reported location to its current end
#define CHPL_LEXER_REPORT_END(SCANNER__, NLINES__, NCOLS__, NAME__, \
                              EINFO__...)                           \
  CHPL_LEXER_REPORT_ACTUAL(SCANNER__, NLINES__, NCOLS__,            \
                           /* MOVE_TO_END__ */ true, NAME__, ##EINFO__)

#define CHPL_LEXER_REPORT_ACTUAL(SCANNER__, NLINES__, NCOLS__, MOVE_TO_END__, \
                                 NAME__, EINFO__...)                          \
  {                                                                           \
    ParserContext* pContext = yyget_extra(SCANNER__);                         \
    YYLTYPE loc = *yyget_lloc(SCANNER__);                                     \
    updateLocation(&loc, NLINES__, NCOLS__);                                  \
    if (MOVE_TO_END__) loc = pContext->makeLocationAtLast(loc);               \
    CHPL_PARSER_REPORT(pContext, NAME__, loc, ##EINFO__);                     \
  }


/**
 * Helper macros to report errors in post-parse-checks to the Builder
 */

#define CHPL_POSTPARSE_REPORT(BUILDER__, NAME__, NODE__, EINFO__...) \
  BUILDER__.addError(                                        \
      CHPL_GET_ERROR(BUILDER__.context(), NAME__, NODE__->id(), EINFO__))

#define CHPL_POSTPARSE_ERR_SIMPLE(BUILDER__, NODE__, MSG__) \
  CHPL_POSTPARSE_REPORT(BUILDER__, PostParseErr, NODE__, MSG__)

#define CHPL_POSTPARSE_WARN_SIMPLE(BUILDER__, NODE__, MSG__) \
  CHPL_POSTPARSE_REPORT(BUILDER__, PostParseWarn, NODE__, MSG__)

#endif
