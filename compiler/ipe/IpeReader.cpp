/*
 * Copyright 2004-2015 Cray Inc.
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

#include "IpeReader.h"

#include "astutil.h"
#include "files.h"
#include "stmt.h"
#include "stringutil.h"

IpeReader::IpeReader()
{
  yylex_init(&(mContext.scanner));

  mParser                       = yypstate_new();

  yyblock                       =  NULL;

  yyfilename                    =  NULL;
  yystartlineno                 =     1;

  mYYlloc.first_line            =     1;
  mYYlloc.first_column          =     0;

  mYYlloc.last_line             =     1;
  mYYlloc.last_column           =     0;

  chplLineno                    =     1;

  chplParseString               = false;
  chplParseStringMsg            =  NULL;

  currentFileNamedOnCommandLine = false;
}

IpeReader::~IpeReader()
{
  currentFileNamedOnCommandLine = false;

  chplParseStringMsg            =  NULL;
  chplParseString               = false;

  chplLineno                    =    -1;

  mYYlloc.last_column           =     0;
  mYYlloc.last_line             =    -1;

  mYYlloc.first_column          =     0;
  mYYlloc.first_line            =    -1;

  yystartlineno                 =    -1;
  yyfilename                    =  NULL;

  yypstate_delete(mParser);

  yylex_destroy(mContext.scanner);
}

// Read one statement
Expr* IpeReader::readStmt()
{
  int   lexerStatus  = 100;
  int   parserStatus = YYPUSH_MORE;
  Expr* retval       = 0;

  mContext.latestComment = NULL;

  while (lexerStatus != 0 && parserStatus == YYPUSH_MORE && retval == 0)
  {
    YYSTYPE yylval;

    mContext.generatedStmt = NULL;

    lexerStatus            = yylex(&yylval, &mYYlloc, mContext.scanner);

    if (lexerStatus >= 0)
      parserStatus = yypush_parse(mParser, lexerStatus, &yylval, &mYYlloc, &mContext);

    else if (lexerStatus == YYLEX_BLOCK_COMMENT)
      mContext.latestComment = yylval.pch;

    if (mContext.generatedStmt != 0)
    {
      if (BlockStmt* bs = toBlockStmt(mContext.generatedStmt))
      {
        if (Expr* expr = bs->body.last())
          retval = expr;
      }
    }
  }

  if (retval != 0)
    insert_help(retval, NULL, NULL);

  return retval;
}
