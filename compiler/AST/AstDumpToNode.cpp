/*
 * Copyright 2004-2014 Cray Inc.
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

#include "AstDumpToNode.h"

#include "driver.h"
#include "expr.h"
#include "flags.h"
#include "log.h"
#include "stmt.h"
#include "stringutil.h"
#include "symbol.h"
#include "type.h"

#include "WhileDoStmt.h"
#include "DoWhileStmt.h"
#include "CForLoop.h"
#include "ForLoop.h"

void AstDumpToNode::view(const char* passName, int passNum)
{
  forv_Vec(ModuleSymbol, module, allModules)
  {
    if (log_module[0] == '\0' || strcmp(log_module, module->name) == 0)
    {
      AstDumpToNode logger;

      if (logger.open(module, passName, passNum) == true)
      {
        module->accept(&logger);
        logger.close();
      }
    }
  }
}

AstDumpToNode::AstDumpToNode(FILE* fp)
{
  mPath      = 0;
  mFP        = fp;

  mOffset    = 0;
  mNeedSpace = false;

  mModule    = 0;
}

AstDumpToNode::AstDumpToNode()
{
  mPath      = 0;
  mFP        = 0;

  mOffset    = 0;
  mNeedSpace = false;

  mModule    = 0;
}

AstDumpToNode::~AstDumpToNode()
{
  if (mPath != 0 && mFP != 0)
  {
    close();
  }
}

bool AstDumpToNode::open(ModuleSymbol* mod, const char* passName, int passNum)
{
  char numBuf[4] = { '\0' };

  snprintf(numBuf, 4, "%02d", passNum);

  mPath   = astr(mod->name, "_", numBuf, passName, ".ast");
  mFP     = fopen(astr(log_dir, mPath), "w");

  mModule = mod;

  if (mFP != 0)
  {
    fprintf(mFP, "AST dump for %s after pass %s.\n", mod->name, passName);
  }

  return (mFP != 0) ? true : false;
}

bool AstDumpToNode::close()
{
  bool retval = false;

  if (mFP != 0)
  {
    retval = (fclose(mFP) == 0) ? true : false;
    mFP    = 0;
  }

  return retval;
}

/******************************** | *********************************
*                                                                   *
* The visit functions for each concrete class                       *
*                                                                   *
* Note that there are default implementations in AstLogger          *
*                                                                   *
********************************* | ********************************/

bool AstDumpToNode::enterModSym(ModuleSymbol* node)
{
  bool retval = false;

  newline();

  // chpl_Program embeds a list of all the modules.
  // Skip the contents of the embedded ones.
  if (mModule                                != 0  &&
      strcmp(mModule->name, "chpl__Program") == 0  &&
      strcmp(node->name,    "chpl__Program") != 0)
  {
    fprintf(mFP, "#<ModuleSymbol %s>", node->name);

    retval = false;

  }
  else
  {
    fprintf(mFP, "#<ModuleSymbol %s", node->name);
    mOffset = mOffset + 2;

    retval =  true;
  }

  return retval;
}

void AstDumpToNode::exitModSym(ModuleSymbol* node)
{
  if (node->modUseList.n > 0) {
    newline();
    fprintf(mFP, "ModUseList:");

    forv_Vec(ModuleSymbol, mod, node->modUseList) {
      fprintf(mFP, " %s", mod->name);
    }
  }

  mOffset = mOffset - 2;
  newline();
  fprintf(mFP, ">\n");
}

//
//
//

bool AstDumpToNode::enterBlockStmt(BlockStmt* node)
{
  char heading[128] = { '\0' };
  bool firstTime    = true;

  newline();

  if (FnSymbol* fn = toFnSymbol(node->parentSymbol))
    if (node == fn->where)
      write(false, "where ", false);

  sprintf(heading, "#<BlockStmt   %12d", node->id);

  write(false, heading, true);

  if (node->blockInfoGet())
  {
    mOffset = mOffset + 2;

    newline();
    write(false, "BlockInfo: ", false);
    mOffset = mOffset + 2;
    node->blockInfoGet()->accept(this);
    mOffset = mOffset - 2;

    mOffset = mOffset - 2;

    newline();
  }

  // Show blockTag bits.
  if (node->blockTag & BLOCK_EXTERN)
    write(true, "extern", true);

  if (node->blockTag & BLOCK_SCOPELESS)
    write(true, "scopeless", true);

  if (node->blockTag & BLOCK_TYPE_ONLY)
    write(true, "type_only", true);

  if (node->blockTag & BLOCK_C_FOR_LOOP)
    write(true, "C_FOR_LOOP", true);

  mOffset = mOffset + 2;

  for_alist(next_ast, node->body)
  {
    if (firstTime == true)
      firstTime = false;
    else
      fprintf(mFP, "\n");

    next_ast->accept(this);
  }

  if (node->modUses)
  {
    newline();
    write(false, "ModUses: ", false);
    node->modUses->accept(this);
  }

  if (node->byrefVars)
  {
    newline();
    write(false, "ByRefVars: ", false);
    node->byrefVars->accept(this);
  }

  mOffset = mOffset - 2;

  newline();
  write(false, ">", true);

  return false;
}

//
//
//

bool AstDumpToNode::enterWhileDoStmt(WhileDoStmt* node)
{
  char heading[128] = { '\0' };
  bool firstTime    = true;

  newline();

  if (FnSymbol* fn = toFnSymbol(node->parentSymbol))
    if (node == fn->where)
      write(false, "where ", false);

  sprintf(heading, "#<WhileDoStmt %12d", node->id);

  write(false, heading, true);

  if (node->condExprGet())
  {
    mOffset = mOffset + 2;

    newline();
    write(false, "CondExpr:", false);
    mOffset = mOffset + 2;
    node->condExprGet()->accept(this);
    mOffset = mOffset - 2;

    mOffset = mOffset - 2;

    newline();
  }

  // Show blockTag bits.
  if (node->blockTag & BLOCK_EXTERN)
    write(false, "extern ", true);

  if (node->blockTag & BLOCK_SCOPELESS)
    write(false, "scopeless ", true);

  if (node->blockTag & BLOCK_TYPE_ONLY)
    write(false, "type_only ", true);

  mOffset = mOffset + 2;

  for_alist(next_ast, node->body)
  {
    if (firstTime == true)
      firstTime = false;
    else
      fprintf(mFP, "\n");

    next_ast->accept(this);
  }

  if (node->modUses)
  {
    newline();
    write(false, "ModUses: ", false);
    node->modUses->accept(this);
  }

  if (node->byrefVars)
  {
    newline();
    write(false, "ByRefVars: ", false);
    node->byrefVars->accept(this);
  }

  mOffset = mOffset - 2;

  newline();
  write(false, ">", true);

  return false;
}

//
//
//

bool AstDumpToNode::enterDoWhileStmt(DoWhileStmt* node)
{
  char heading[128] = { '\0' };
  bool firstTime    = true;

  newline();

  if (FnSymbol* fn = toFnSymbol(node->parentSymbol))
    if (node == fn->where)
      write(false, "where ", false);

  sprintf(heading, "#<DoWhileStmt %12d", node->id);

  write(false, heading, true);

  if (node->condExprGet())
  {
    mOffset = mOffset + 2;

    newline();
    write(false, "CondExpr:", false);
    mOffset = mOffset + 2;
    node->condExprGet()->accept(this);
    mOffset = mOffset - 2;

    mOffset = mOffset - 2;

    newline();
  }

  // Show blockTag bits.
  if (node->blockTag & BLOCK_EXTERN)
    write(false, "extern ", true);

  if (node->blockTag & BLOCK_SCOPELESS)
    write(false, "scopeless ", true);

  if (node->blockTag & BLOCK_TYPE_ONLY)
    write(false, "type_only ", true);

  mOffset = mOffset + 2;

  for_alist(next_ast, node->body)
  {
    if (firstTime == true)
      firstTime = false;
    else
      fprintf(mFP, "\n");

    next_ast->accept(this);
  }

  if (node->modUses)
  {
    newline();
    write(false, "ModUses: ", false);
    node->modUses->accept(this);
  }

  if (node->byrefVars)
  {
    newline();
    write(false, "ByRefVars: ", false);
    node->byrefVars->accept(this);
  }

  mOffset = mOffset - 2;

  newline();
  write(false, ">", true);

  return false;
}

//
//
//

bool AstDumpToNode::enterCForLoop(CForLoop* node)
{
  char heading[128] = { '\0' };
  bool firstTime    = true;

  newline();

  if (FnSymbol* fn = toFnSymbol(node->parentSymbol))
    if (node == fn->where)
      write(false, "where ", false);

  sprintf(heading, "#<CForLoop    %12d", node->id);

  write(false, heading, true);

  if (node->blockInfoGet())
  {
    mOffset = mOffset + 2;

    newline();
    write(false, "BlockInfo: ", false);
    mOffset = mOffset + 2;
    node->blockInfoGet()->accept(this);
    mOffset = mOffset - 2;

    mOffset = mOffset - 2;

    newline();
  }

  // Show blockTag bits.
  if (node->blockTag & BLOCK_EXTERN)
    write(false, "extern ", true);
  if (node->blockTag & BLOCK_SCOPELESS)
    write(false, "scopeless ", true);
  if (node->blockTag & BLOCK_TYPE_ONLY)
    write(false, "type_only ", true);

  mOffset = mOffset + 2;

  for_alist(next_ast, node->body)
  {
    if (firstTime == true)
      firstTime = false;
    else
      fprintf(mFP, "\n");

    next_ast->accept(this);
  }

  if (node->modUses)
  {
    newline();
    write(false, "ModUses: ", false);
    node->modUses->accept(this);
  }

  if (node->byrefVars)
  {
    newline();
    write(false, "ByRefVars: ", false);
    node->byrefVars->accept(this);
  }

  mOffset = mOffset - 2;

  newline();
  write(false, ">", true);

  return false;
}


//
//
//

bool AstDumpToNode::enterForLoop(ForLoop* node)
{
  char heading[128] = { '\0' };
  bool firstTime    = true;

  newline();

  if (FnSymbol* fn = toFnSymbol(node->parentSymbol))
    if (node == fn->where)
      write(false, "where ", false);

  sprintf(heading, "#<ForLoop     %12d", node->id);

  write(false, heading, true);

  if (node->blockInfoGet())
  {
    mOffset = mOffset + 2;

    newline();
    write(false, "BlockInfo: ", false);
    mOffset = mOffset + 2;
    node->blockInfoGet()->accept(this);
    mOffset = mOffset - 2;

    mOffset = mOffset - 2;

    newline();
  }

  // Show blockTag bits.
  if (node->blockTag & BLOCK_EXTERN)
    write(false, "extern", true);

  if (node->blockTag & BLOCK_SCOPELESS)
    write(false, "scopeless", true);

  if (node->blockTag & BLOCK_TYPE_ONLY)
    write(false, "type_only", true);

  mOffset = mOffset + 2;

  for_alist(next_ast, node->body)
  {
    if (firstTime == true)
      firstTime = false;
    else
      fprintf(mFP, "\n");

    next_ast->accept(this);
  }

  if (node->modUses)
  {
    newline();
    write(false, "ModUses: ", false);
    node->modUses->accept(this);
  }

  if (node->byrefVars)
  {
    newline();
    write(false, "ByRefVars: ", false);
    node->byrefVars->accept(this);
  }

  mOffset = mOffset - 2;

  newline();
  write(false, ">", true);

  return false;
}


//
//
//

bool AstDumpToNode::enterDefExpr(DefExpr* node)
{
  newline();
  fprintf(mFP, "#<DefExpr      %12d", node->id);

  mOffset = mOffset + 2;

  if (false)
  {

  }

  else if (node->sym == 0)
  {

  }

  else if (toArgSymbol(node->sym))
  {

  }

  else if (toEnumSymbol(node->sym))
  {
    ast_symbol("def", node->sym, true);
  }

  else if (toFnSymbol(node->sym))
  {

  }

  else if (toLabelSymbol(node->sym))
  {

  }

  else if (toModuleSymbol(node->sym) != 0)
  {

  }

  else if (toTypeSymbol(node->sym))
  {
    if (toAggregateType(node->sym->type))
    {
      if (node->sym->hasFlag(FLAG_SYNC))
        write("sync");

      if (node->sym->hasFlag(FLAG_SINGLE))
        write("single");
    }

    ast_symbol("type", node->sym, true);
  }

  else if (toVarSymbol(node->sym))
  {

  }

  if (node->sym)
  {
    newline();
    fprintf(mFP, "sym:");

    mOffset = mOffset + 2;
    node->sym->accept(this);
    mOffset = mOffset - 2;
  }

  if (node->exprType)
  {
    newline();
    write("exprType:");

    mOffset = mOffset + 2;
    node->exprType->accept(this);
    mOffset = mOffset - 2;
  }

  if (node->init)
  {
    newline();
    write("init:");

    mOffset = mOffset + 2;
    node->init->accept(this);
    mOffset = mOffset - 2;
  }

  mOffset = mOffset - 2;
  newline();
  write(false, ">", false);

  return false;
}

//
//
//

bool AstDumpToNode::enterFnSym(FnSymbol* node)
{
  newline();
  fprintf(mFP, "#<FnSymbol %12d", node->id);

  mOffset = mOffset + 2;

  if (node->_this && node->_this->defPoint)
  {
    newline();
    write(false, "DefPoint: ", false);
    mOffset = mOffset + 2;
    ast_symbol(node->_this->type->symbol, false);
    mOffset = mOffset - 2;
  }

  newline();
  fprintf(mFP, "Name:     %s ", node->name);

  // Now the return type info
  switch (node->retTag)
  {
    case RET_VALUE:
      break;

    case RET_REF:
      newline();
      write("RetTag:   ref");
      break;

    case RET_PARAM:
      newline();
      write("RetTag:   param");
      break;

    case RET_TYPE:
      newline();
      write("RetTag:   type");
      break;
  }

  if (node->retType && node->retType->symbol)
  {
    newline();
    fprintf(mFP, "RetType:");
    mOffset = mOffset + 2;
    node->retType->symbol->accept(this);
    mOffset = mOffset - 2;
  }

  newline();
  write(false, "Formals: ", false);

  mOffset = mOffset + 2;

  for_alist(next_ast, node->formals)
  {
    next_ast->accept(this);
  }

  mOffset = mOffset - 2;

  if (node->setter)
  {
    newline();
    write(false, "Setter: ", false);

    mOffset = mOffset + 2;
    node->setter->accept(this);
    mOffset = mOffset - 2;
  }

  if (node->body)
  {
    newline();
    write(false, "Body: ", false);

    mOffset = mOffset + 2;
    node->body->accept(this);
    mOffset = mOffset - 2;
  }

  if (node->where)
  {
    newline();
    write(false, "Where: ", false);
    node->where->accept(this);
  }

  if (node->retExprType)
  {
    newline();
    write(false, "RetExprType: ", false);

    mOffset = mOffset + 2;
    node->retExprType->accept(this);
    mOffset = mOffset - 2;
  }

  mOffset = mOffset - 2;

  newline();
  fprintf(mFP, ">");

  return false;
}

//
//
//

bool AstDumpToNode::enterCallExpr(CallExpr* node)
{
  newline();
  fprintf(mFP, "#<CallExpr    %12d", node->id);
  mOffset = mOffset + 2;
  newline();

  if (FnSymbol* fn = node->isResolved())
  {
    if (fn->hasFlag(FLAG_BEGIN_BLOCK))
      write("begin");

    else if (fn->hasFlag(FLAG_ON_BLOCK))
      write("on");
  }

  if (false)
  {

  }

  else if (node->primitive == 0)
  {
    write("call");
  }

  else if (node->isPrimitive(PRIM_RETURN))
  {
    write("return");
  }

  else if (node->isPrimitive(PRIM_YIELD))
  {
    write("yield ");
  }

  else
  {
    write(node->primitive->name);
  }

  if (node->partialTag)
    write("(partial)");

  return true;
}

void AstDumpToNode::exitCallExpr(CallExpr* node) {
  mOffset = mOffset - 2;
  newline();
  fprintf(mFP, ">");
}

//
//
//

bool AstDumpToNode::enterNamedExpr(NamedExpr* node)
{
  newline();
  fprintf(mFP, "#<NamedExpr");

  fprintf(mFP, "(%s =", node->name);
  mNeedSpace = true;

  return true;
}

void AstDumpToNode::exitNamedExpr(NamedExpr* node)
{
  write(false, ")", true);
}

//
//
//

void AstDumpToNode::visitSymExpr(SymExpr* node)
{
  Symbol* sym = node->var;

  newline();

  fprintf(mFP, "#<SymExpr     %12d var: ", node->id);
  writeSymbol(sym);
  fprintf(mFP, ">");
}

//
//
//

void AstDumpToNode::visitUsymExpr(UnresolvedSymExpr* node)
{
  newline();
  fprintf(mFP, "#<UnresolvedSymExpr \"%s\">", node->unresolved);
}

//
//
//

bool AstDumpToNode::enterCondStmt(CondStmt* node)
{
  newline();
  fprintf(mFP, "#<CondStmt");

  mOffset = mOffset + 2;
  newline();
  fprintf(mFP, "cond:");

  mOffset = mOffset + 2;
  node->condExpr->accept(this);
  mOffset = mOffset - 2;

  newline();
  fprintf(mFP, "consequent:");

  mOffset = mOffset + 2;
  node->thenStmt->accept(this);
  mOffset = mOffset - 2;

  if (node->elseStmt)
  {
    newline();
    fprintf(mFP, "alternative:");

    mOffset = mOffset + 2;
    node->elseStmt->accept(this);
    mOffset = mOffset - 2;
  }

  mOffset = mOffset - 2;
  newline();
  fprintf(mFP, ">");

  return false;
}


//
//
//

void AstDumpToNode::visitEblockStmt(ExternBlockStmt* node)
{
  newline();

  fprintf(mFP, "#<ExternBlockStmt");
  fprintf(mFP, "(%s", node->astTagAsString());
  fprintf(mFP, ">");
}

//
//
//

bool AstDumpToNode::enterGotoStmt(GotoStmt* node)
{
  newline();
  fprintf(mFP, "#<GotoStmt ");

  mOffset = mOffset + 2;
  newline();

  switch (node->gotoTag)
  {
    case GOTO_NORMAL:
      fprintf(mFP, "tag:   goto");
      break;

    case GOTO_BREAK:
      fprintf(mFP, "tag:   break");
      break;

    case GOTO_CONTINUE:
      fprintf(mFP, "tag:   continue");
      break;

    case GOTO_RETURN:
      fprintf(mFP, "tag:   gotoReturn");
      break;

    case GOTO_GETITER_END:
      fprintf(mFP, "tag:   gotoGetiterEnd");
      break;

    case GOTO_ITER_RESUME:
      fprintf(mFP, "tag:   gotoIterResume");
      break;

    case GOTO_ITER_END:
      fprintf(mFP, "tag:   gotoIterEnd");
      break;
  }

  if (SymExpr* label = toSymExpr(node->label))
  {
    if (label->var != gNil)
    {
      newline();
      fprintf(mFP, "label: ");
      mOffset = mOffset + 2;
      ast_symbol(label->var, true);
      mOffset = mOffset - 2;
    }
  }

  mOffset = mOffset - 2;
  newline();
  fprintf(mFP, ">");

  return false;
}

void AstDumpToNode::exitGotoStmt(GotoStmt* node)
{

}

//
//
//

bool AstDumpToNode::enterAggrType(AggregateType* node)
{
  newline();
  fprintf(mFP, "#<AggregateType %s ", node->symbol->name);
  mOffset = mOffset + 2;

  return true;
}

void AstDumpToNode::exitAggrType(AggregateType* node)
{
  mOffset = mOffset - 2;
  newline();
  fprintf(mFP, ">");
}

//
//
//

bool AstDumpToNode::enterEnumType(EnumType* node)
{
  newline();
  fprintf(mFP, "#<EnumType");
  mOffset = mOffset + 2;
  return true;
}

void AstDumpToNode::exitEnumType(EnumType* node)
{
  mOffset = mOffset - 2;
  newline();
  fprintf(mFP, ">");
}

//
//
//

void AstDumpToNode::visitPrimType(PrimitiveType* node)
{
  newline();
  fprintf(mFP, "#<PrimitiveType %s>", node->symbol->name);
}

//
//
//

bool AstDumpToNode::enterArgSym(ArgSymbol* node)
{
  newline();
  fprintf(mFP, "#<ArgSymbol  ");

  mOffset = mOffset + 2;

  ast_symbol(node, true);

  if (node->typeExpr)
  {
    newline();
    fprintf(mFP, "typeExpr: ");
    mOffset = mOffset + 2;
    node->typeExpr->accept(this);
    mOffset = mOffset - 2;
  }

  if (node->defaultExpr)
  {
    newline();
    fprintf(mFP, "defaultExpr: ");
    mOffset = mOffset + 2;
    node->defaultExpr->accept(this);
    mOffset = mOffset - 2;
  }

  if (node->variableExpr)
  {
    newline();
    fprintf(mFP, "variableExpr: ");
    mOffset = mOffset + 2;
    node->variableExpr->accept(this);
    mOffset = mOffset - 2;
  }

  mOffset = mOffset - 2;
  newline();
  fprintf(mFP, ">");

  return false;
}

//
//
//

void AstDumpToNode::visitEnumSym(EnumSymbol* node)
{
  newline();
  fprintf(mFP, "#<EnumSymbol>\n");
}

//
//
//

void AstDumpToNode::visitLabelSym(LabelSymbol* node)
{
  newline();
  fprintf(mFP, "#<LabelSymbol ");
  mOffset = mOffset + 2;
  ast_symbol(node, true);
  mOffset = mOffset - 2;
  fprintf(mFP, ">");
}

//
//
//

bool AstDumpToNode::enterTypeSym(TypeSymbol* node)
{
  newline();

  fprintf(mFP, "#<TypeSymbol name: %s type: ", node->name);
  mOffset = mOffset + 2;
  node->type->accept(this);
  mOffset = mOffset - 2;
  newline();
  fprintf(mFP, ">");

  return false;
}

//
//
//

void AstDumpToNode::visitVarSym(VarSymbol* node)
{
  newline();
  writeSymbol(node);
}

/******************************** | *********************************
*                                                                   *
* Helper routines                                                   *
*                                                                   *
********************************* | ********************************/

void AstDumpToNode::writeSymbol(Symbol* sym) const
{
  char          name[1024];
  ModuleSymbol* mod        = sym->getModule();

  if (mod != 0)
  {
    if (false)
      ;

    else if (sym == 0)
      sprintf(name, "??:NULL");

    else if (mod->name == 0 && sym->name == 0)
      sprintf(name, "??:??");

    else if (mod->name != 0 && sym->name == 0)
      sprintf(name, "%s:??", mod->name);

    else if (mod->name == 0 && sym->name != 0)
      sprintf(name, "??:%s", sym->name);

    else
      sprintf(name, "%s.%s", mod->name, sym->name);
  }
  else
  {
    if (sym->name == 0)
      sprintf(name, "NULL:??");

    else
      sprintf(name, "NULL:%s", sym->name);
  }



  if (false)
  {

  }

  else if (isArgSymbol(sym) == true)
  {
    fprintf(mFP, "#<ArgSymbol    %12d name: %-36s", sym->id, name);

    if (sym->type != 0)
    {
      fprintf(mFP, " type:   ");
      writeType(sym->type);
    }

    writeFlags(mFP, sym);
    fprintf(mFP, ">");

  }

  else if (isEnumSymbol(sym) == true)
  {
    fprintf(mFP, "#<EnumSymbol   %12d name: %-36s", sym->id, name);

    if (sym->type != 0)
    {
      fprintf(mFP, " type:   ");
      writeType(sym->type);
    }

    writeFlags(mFP, sym);
    fprintf(mFP, ">");

  }

  else if (isFnSymbol(sym) == true)
  {
    fprintf(mFP, "#<FnSymbol     %12d name: %-36s", sym->id, name);

    if (sym->type != 0)
    {
      fprintf(mFP, " type:   ");
      writeType(sym->type);
    }

    writeFlags(mFP, sym);
    fprintf(mFP, ">");

  }

  else if (isLabelSymbol(sym) == true)
  {
    fprintf(mFP, "#<LabelSymbol  %12d name: %-36s", sym->id, name);

    if (sym->type != 0)
    {
      fprintf(mFP, " type:   ");
      writeType(sym->type);
    }

    writeFlags(mFP, sym);
    fprintf(mFP, ">");

  }

  else if (isModuleSymbol(sym) == true)
  {
    fprintf(mFP, "#<ModuleSymbol %12d name: %-36s", sym->id, name);

    if (sym->type != 0)
    {
      fprintf(mFP, " type:   ");
      writeType(sym->type);
    }

    writeFlags(mFP, sym);
    fprintf(mFP, ">");


  }

  else if (isTypeSymbol(sym) == true)
  {
    fprintf(mFP, "#<TypeSymbol   %12d name: %-36s", sym->id, name);

    if (sym->type != 0)
    {
      fprintf(mFP, " type:   ");
      writeType(sym->type);
    }

    writeFlags(mFP, sym);
    fprintf(mFP, ">");

  }

  else if (VarSymbol* var = toVarSymbol(sym))
  {
    if (var->immediate != 0)
    {
      const size_t bufSize = 128;
      char         imm[bufSize];

      snprint_imm(imm, bufSize, *var->immediate);

      if (is_imag_type(var->type) == true)
      {
        char* tail = strchr(imm, '\0');

        *tail++ = 'i';
        *tail   = '\0';
      }

      fprintf(mFP, "#<VarSymbol    %12d imm:  %-36s", var->id, imm);

      if (sym->type)
      {
        fprintf(mFP, " type:   ");
        writeType(sym->type);
      }

      writeFlags(mFP, sym);

      fprintf(mFP, ">");

    }

    else
    {
      fprintf(mFP, "#<VarSymbol    %12d name: %-36s", var->id, name);

      if (sym->type)
      {
        fprintf(mFP, " type:   ");
        writeType(sym->type);
      }

      writeFlags(mFP, sym);
      fprintf(mFP, ">");
    }
  }
}

void AstDumpToNode::ast_symbol(const char* tag, Symbol* sym, bool def)
{
  fprintf(mFP, "%s ", tag);
  ast_symbol(sym, def);
}


void AstDumpToNode::ast_symbol(Symbol* sym, bool def)
{
  ModuleSymbol* mod = sym->getModule();

  if (def)
  {
    if (ArgSymbol* arg = toArgSymbol(sym))
    {
      newline();

      switch (arg->intent)
      {
        case INTENT_IN:        write("intent: in arg ");        break;
        case INTENT_INOUT:     write("intent: inout arg ");     break;
        case INTENT_OUT:       write("intent: out arg ");       break;
        case INTENT_CONST:     write("intent: const arg ");     break;
        case INTENT_CONST_IN:  write("intent: const in arg ");  break;
        case INTENT_CONST_REF: write("intent: const ref arg "); break;
        case INTENT_REF:       write("intent: ref arg ");       break;
        case INTENT_PARAM:     write("intent: param arg ");     break;
        case INTENT_TYPE:      write("intent: type arg ");      break;
        case INTENT_BLANK:     write("intent: arg ");           break;
      }
    }
  }

  newline();

  if ((mod == 0 || mod->name == 0) && sym->name == 0)
  {
    fprintf(mFP, "name:   %s.%s", "??", "??");
  }

  else if ((mod == 0 || mod->name == 0) && sym->name != 0)
  {
    fprintf(mFP, "name:   %s.%s", "??", sym->name);
  }

  else if (mod->name != 0 && sym->name == 0)
  {
    fprintf(mFP, "name:   %s.%s", mod->name, "??");
  }

  else
  {
    fprintf(mFP, "name:   %s.%s", mod->name, sym->name);
  }

  if (fLogIds)
    fprintf(mFP, "[%d]", sym->id);

  if (sym->type)
  {
    newline();
    fprintf(mFP, "type:   ");
    writeType(sym->type);
  }

  if (def                &&
      !toTypeSymbol(sym) &&
      sym->type          &&
      sym->type->symbol  &&
      sym->type != dtUnknown)
  {
    newline();
    write(false, "type->symbol : ", false);
    ast_symbol(sym->type->symbol, false);
  }

  if (sym->hasFlag(FLAG_GENERIC))
    write(false, "?", false);

  mNeedSpace = true;
}

void AstDumpToNode::writeType(Type* type) const
{
  if (false)
    ;

  else if (type == 0)
    ;

  else if (PrimitiveType* t = toPrimitiveType(type))
    fprintf(mFP, "#<PrimitiveType %s>", t->symbol->name);

  else if (AggregateType* t = toAggregateType(type))
    fprintf(mFP, "#<AggregateType %s>", t->symbol->name);

  else if (EnumType*      t = toEnumType(type))
    fprintf(mFP, "#<EnumType      %s>", t->symbol->name);

  else
    USR_FATAL("This cannot happen");
}

void AstDumpToNode::write(const char* text)
{
  write(true, text, true);
}

void AstDumpToNode::write(bool spaceBefore, const char* text, bool spaceAfter)
{
  if (spaceBefore == true && mNeedSpace == true)
    fputc(' ', mFP);

  fputs(text, mFP);

  mNeedSpace = spaceAfter;
}

void AstDumpToNode::newline() {
  fputc('\n', mFP);

  for (int i = 0; i < mOffset; ++i)
    fputc(' ', mFP);

  mNeedSpace = false;
}
