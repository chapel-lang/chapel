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

#include "IpeSequence.h"

#include "AstDumpToNode.h"

IpeSequence::IpeSequence(const std::vector<Expr*>& stmts) : BlockStmt(NULL, BLOCK_SCOPELESS)
{
  for (size_t i = 0; i < stmts.size(); i++)
    insertAtTail(stmts[i]);
}

IpeSequence::IpeSequence(const std::vector<Expr*>& stmts,
                         BlockTag                  tag) : BlockStmt(NULL, tag)
{
  for (size_t i = 0; i < stmts.size(); i++)
    insertAtTail(stmts[i]);
}

IpeSequence::IpeSequence(BlockTag tag) : BlockStmt(NULL, tag)
{

}

IpeSequence::~IpeSequence()
{

}

bool IpeSequence::isScopeless() const
{
  return true;
}

void IpeSequence::describe(int offset)
{
  AstDumpToNode logger(stdout, offset + 3);
  char          pad[32] = { '\0' };

  if (offset < 32)
  {
    char* tptr = pad;

    for (int i = 0; i < offset; i++)
      *tptr++ = ' ';

    *tptr = '\0';
  }

  printf("%s#<IpeSequence\n", pad);

  for (int i = 1; i <= body.length; i++)
  {
    Expr* expr = body.get(i);

    if (isBlockStmt(expr) == true)
    {
      IpeSequence* seq = (IpeSequence*) expr;

      seq->describe(offset + 3);
    }
    else
    {
      printf("%s   ", pad);
      expr->accept(&logger);
      printf("\n");
    }
  }


  printf("%s>\n", pad);
}
