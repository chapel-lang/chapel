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

#ifndef CHPL_PARSING_PARSER_ERROR_H
#define CHPL_PARSING_PARSER_ERROR_H

/**
  Helper macro to report errors from the parser, which takes care of saving the
  error in the ParserContext, retrieving the global Context from the
  ParserContext, and converting YYLTYPE locations.

  Evaluates to an ErroneousExpression error sentinel at the location of the
  error, which may be used or ignored.
 */
#define CHPL_PARSER_REPORT(P_CONTEXT__, NAME__, LOC__, EINFO__...) \
  P_CONTEXT__->report(LOC__,                                       \
    CHPL_PARSER_GET_ERROR(P_CONTEXT__, NAME__, LOC__, ##EINFO__))
#define CHPL_PARSER_GET_ERROR(P_CONTEXT__, NAME__, LOC__, EINFO__...) \
  CHPL_GET_ERROR(NAME__, P_CONTEXT__->convertLocation(LOC__), ##EINFO__)

/**
  Helper macros to report errors from the lexer, including retrieving the
  global Context and lexer-specific location adjustments.

  Errors reported via this are forwarded on to parser error reporting.
 */
#define CHPL_LEXER_REPORT(SCANNER__, NLINES__, NCOLS__, NAME__, EINFO__...) \
  CHPL_LEXER_REPORT_ACTUAL(SCANNER__, NLINES__, NCOLS__,                    \
                           /* MOVE_TO_END__ */ false, NAME__, ##EINFO__)

// this variant moves the beginning of the reported location to its current end
#define CHPL_LEXER_REPORT_END(SCANNER__, NLINES__, NCOLS__, NAME__, \
                              EINFO__...)                           \
  CHPL_LEXER_REPORT_ACTUAL(SCANNER__, NLINES__, NCOLS__,            \
                           /* MOVE_TO_END__ */ true, NAME__, ##EINFO__)

#define CHPL_LEXER_REPORT_ACTUAL(SCANNER__, NLINES__, NCOLS__, MOVE_TO_END__,  \
                                 NAME__, EINFO__...)                           \
  CHPL_PARSER_REPORT(yyget_extra(SCANNER__), NAME__,                           \
                     getLocation(SCANNER__, NLINES__, NCOLS__, MOVE_TO_END__), \
                     ##EINFO__);

#endif
