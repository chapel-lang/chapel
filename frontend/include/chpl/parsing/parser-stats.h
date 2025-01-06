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

#ifndef CHPL_PARSING_PARSER_STATS_H
#define CHPL_PARSING_PARSER_STATS_H

namespace chpl {

namespace parsing {

/*
  A place to collect code stats and generate a report of the number of lines
  which are code, comments, and blank lines.
*/
struct ParserStats {

 /// \cond DO_NOT_DOCUMENT
  bool countTokens;
  bool printTokens;
  static const int HIST_SIZE = 4096;
  int lineTokens;
  int fileTokens;
  int totTokens;
  int blankLines;
  int commentLines;
  int codeLines;
  int fileTokenHist[HIST_SIZE];
  int totTokenHist[HIST_SIZE];
  int maxTokensPerLineInFile;
  int maxTokensPerLineTot;

  int firstLineInFile;
  int lineBlank;
  int lineComment;
  std::string line;

  bool tokenCountingOn = false;

  void clearHist(int h[]);

  void clearLine();

  void clearFile();

  void clearGlob();

  void printSeparator();

  void printFileSummary();

  void initTokenCount();

  void startCountingFileTokens(const char* filename);
  void stopCountingFileTokens();

  void finishCountingTokens();

  void countToken(const char* toktext1,
                  const char* toktext2 = NULL,
                  const char* toktext3 = NULL);

  void countNewline();

  void countCommentLine();

  void countSingleLineComment(const char* comment);

  void countMultiLineComment(const char* comment);
/// \endcond
  ParserStats();
  ParserStats(bool printTokens);
};

} // end namespace parsing

} // end namespace chpl


#endif
