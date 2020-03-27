/*
 * Copyright 2020 Hewlett Packard Enterprise Development LP
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

#ifndef __STDC_FORMAT_MACROS
#define __STDC_FORMAT_MACROS
#endif

#include "AstDumpToHtml.h"

#include "expr.h"
#include "ImportStmt.h"
#include "log.h"
#include "LoopExpr.h"
#include "runpasses.h"
#include "stmt.h"
#include "stringutil.h"
#include "symbol.h"

#include "WhileDoStmt.h"
#include "DoWhileStmt.h"
#include "CForLoop.h"
#include "ForLoop.h"
#include "ParamForLoop.h"

#include <cstdio>
#include <inttypes.h>

int   AstDumpToHtml::sPassIndex = 1;
FILE* AstDumpToHtml::sIndexFP   = 0;

const char* HTML_DL_open_tag = "<DL><DD>";
const char* HTML_DL_close_tag = "</DD></DL>";

AstDumpToHtml::AstDumpToHtml() {
  mFP = 0;
}

AstDumpToHtml::~AstDumpToHtml() {
  close();
}

void AstDumpToHtml::init() {
  if (!(sIndexFP = fopen(astr(log_dir, "index.html"), "w"))) {
    USR_FATAL("cannot open html index file \"%s\" for writing", astr(log_dir, "index.html"));
  }

  fprintf(sIndexFP, "<HTML>\n");
  fprintf(sIndexFP, "<HEAD>\n");
  fprintf(sIndexFP, "<TITLE> Compilation Dump </TITLE>\n");
  fprintf(sIndexFP, "<SCRIPT SRC=\"https://chapel-lang.org/developer/mktree.js\" LANGUAGE=\"JavaScript\"></SCRIPT>");
  fprintf(sIndexFP, "<LINK REL=\"stylesheet\" HREF=\"https://chapel-lang.org/developer/mktree.css\">\n");
  fprintf(sIndexFP, "<STYLE>\n");
  fprintf(sIndexFP, "a.userMod:link {color:#00A0FF;}\n");
  fprintf(sIndexFP, "a.userMod:visited {color:#A000F0;}\n");
  fprintf(sIndexFP, "</STYLE>\n");
  fprintf(sIndexFP, "</HEAD>\n");
  fprintf(sIndexFP, "<div style=\"text-align: center;\"><big><big><span style=\"font-weight: bold;\">");
  fprintf(sIndexFP, "Compilation Dump<br><br></span></big></big>\n");
  fprintf(sIndexFP, "<div style=\"text-align: left;\">\n\n");

  fprintf(sIndexFP, "<TABLE CELLPADDING=\"0\" CELLSPACING=\"0\">");
}

void AstDumpToHtml::done() {
  fprintf(sIndexFP, "</TABLE>");
  fprintf(sIndexFP, "</HTML>\n");

  fclose(sIndexFP);
}

void AstDumpToHtml::view(const char* passName) {

  fprintf(sIndexFP, "<TR><TD>");
  fprintf(sIndexFP,
          "%s%s[%d]",
          passName,
          fdump_html_include_system_modules ? "<br>" : " ", lastNodeIDUsed());
  fprintf(sIndexFP, "</TD><TD>");

  forv_Vec(ModuleSymbol, module, allModules) {
    if (fdump_html_include_system_modules == true      ||
        module->modTag                    == MOD_USER) {
      AstDumpToHtml logger;

      if (logger.open(module, passName) == true) {
        for_alist(stmt, module->block->body)
          stmt->accept(&logger);

        logger.close();
      }
    }
  }

  fprintf(sIndexFP, "</TD></TR>");
  // add a little bit of space between passes in the index file
  fprintf(sIndexFP, "<TR height=6><TD></TD></TR>");
  fflush(sIndexFP);

  sPassIndex++;
}

bool AstDumpToHtml::open(ModuleSymbol* module, const char* passName) {
  const char* name = html_file_name(sPassIndex, module->name);
  const char* path = astr(log_dir, name);

  mFP = fopen(path, "w");

  if (mFP != 0) {
    const char* modClass = "";

    if (module->modTag == MOD_USER)
      modClass = "class=\"userMod\"";

    fprintf(sIndexFP, "&nbsp;&nbsp;<a %s href=\"%s\">%s</a>\n",
            modClass, name, module->name);
    fprintf(mFP, "<CHPLTAG=\"%s\">\n", passName);
    fprintf(mFP, "<HTML>\n");
    fprintf(mFP, "<HEAD>\n");
    fprintf(mFP, "<TITLE> AST for Module %s after Pass %s </TITLE>\n", module->name, passName);
    fprintf(mFP, "<SCRIPT SRC=\"https://chapel-lang.org/developer/mktree.js\" LANGUAGE=\"JavaScript\"></SCRIPT>\n");
    fprintf(mFP, "<LINK REL=\"stylesheet\" HREF=\"https://chapel-lang.org/developer/mktree.css\">\n");
    fprintf(mFP, "</HEAD><BODY%s>\n",
           fdump_html_wrap_lines ? "" : " style=\"white-space: nowrap;\"");

    if (currentPassNo > 1)
      fprintf(mFP, "<A HREF=%s>previous pass</A> &nbsp;\n", html_file_name(currentPassNo - 1, module->name));

    if (true)
      fprintf(mFP, "<A HREF=%s>next pass</A>\n",            html_file_name(currentPassNo + 1, module->name));

    fprintf(mFP, "<div style=\"text-align: center;\"><big><big><span style=\"font-weight: bold;\">");
    fprintf(mFP, "AST for Module %s after Pass %s <br><br></span></big></big>\n", module->name, passName);
    fprintf(mFP, "<div style=\"text-align: left;\">\n\n");
    fprintf(mFP, "<B>module \n");

    writeSymbol(module, true);

    fprintf(mFP, "</B>\n");
  }

  return (mFP != 0) ? true : false;
}

bool AstDumpToHtml::close() {
  bool retval = false;

  if (mFP != 0) {
    fprintf(mFP, "</BODY></HTML>\n");

    retval = (fclose(mFP) == 0) ? true : false;
    mFP    = 0;
  }

  return retval;
}

//
// CallExpr
//
bool AstDumpToHtml::enterCallExpr(CallExpr* node) {
  if (isBlockStmt(node->parentExpr)) {
    fprintf(mFP, "%s\n", HTML_DL_open_tag);
  }

  fprintf(mFP, " ");

  if (FnSymbol* fn = node->theFnSymbol()) {
    if (fn->hasFlag(FLAG_BEGIN_BLOCK))
      fprintf(mFP, "begin ");
    else if (fn->hasFlag(FLAG_ON_BLOCK))
      fprintf(mFP, "on ");
  }

  fprintf(mFP, "(%d ", node->id);

  if (!node->primitive) {
    fprintf(mFP, "<B>call</B> ");

  } else if (node->isPrimitive(PRIM_RETURN)) {
    fprintf(mFP, "<B>return</B> ");

  } else if (node->isPrimitive(PRIM_YIELD)) {
    fprintf(mFP, "<B>yield</B> ");

  } else {
    fprintf(mFP, "'%s' ", node->primitive->name);

  }

  if (node->partialTag)
    fprintf(mFP, "(partial) ");

  return true;
}

void AstDumpToHtml::exitCallExpr(CallExpr* node) {
  fprintf(mFP, ")");

  if (isBlockStmt(node->parentExpr)) {
    fprintf(mFP, "%s\n", HTML_DL_close_tag);
  }
}


//
// DefExpr
//
bool AstDumpToHtml::enterDefExpr(DefExpr* node) {
  bool retval = true;

  if (isBlockStmt(node->parentExpr)) {
    fprintf(mFP, "%s\n", HTML_DL_open_tag);
  }

  fprintf(mFP, " ");

  if (FnSymbol* fn = toFnSymbol(node->sym)) {
    fprintf(mFP, "<UL CLASS =\"mktree\">\n<LI>");

    adjacent_passes(fn);

    fprintf(mFP, "<CHPLTAG=\"FN%d\">\n", fn->id);
    fprintf(mFP, "<B>function ");

    writeFnSymbol(fn);

    fprintf(mFP, "</B><UL>\n");

  } else if (isTypeSymbol(node->sym)) {
    if (isAggregateType(node->sym->type)) {
      fprintf(mFP, "<UL CLASS =\"mktree\">\n");
      fprintf(mFP, "<LI>");

      if (node->sym->hasFlag(FLAG_SYNC))
        fprintf(mFP, "<B>sync</B> ");

      if (node->sym->hasFlag(FLAG_SINGLE))
        fprintf(mFP, "<B>single</B> ");

      fprintf(mFP, "<B>type ");
      writeSymbol(node->sym, true);
      fprintf(mFP, "</B><UL>\n");

    } else {
      fprintf(mFP, "<B>type </B> ");
      writeSymbol(node->sym, true);
    }

  } else if (VarSymbol* vs = toVarSymbol(node->sym)) {
    if (isSyncType(vs->type)) {
      fprintf(mFP, "<B>sync </B>");

    } else if (isSingleType(vs->type)) {
      fprintf(mFP, "<B>single </B>");
    }

    fprintf(mFP, "<B>var </B> ");
    writeSymbol(node->sym, true);

  } else if (ArgSymbol* s = toArgSymbol(node->sym)) {
    switch (s->intent) {
      case INTENT_IN:        fprintf(mFP, "<B>in</B> ");        break;
      case INTENT_INOUT:     fprintf(mFP, "<B>inout</B> ");     break;
      case INTENT_OUT:       fprintf(mFP, "<B>out</B> ");       break;
      case INTENT_CONST:     fprintf(mFP, "<B>const</B> ");     break;
      case INTENT_CONST_IN:  fprintf(mFP, "<B>const in</B> ");  break;
      case INTENT_CONST_REF: fprintf(mFP, "<B>const ref</B> "); break;
      case INTENT_REF_MAYBE_CONST: fprintf(mFP, "<B>const? ref</B> "); break;
      case INTENT_REF:       fprintf(mFP, "<B>ref</B> ");       break;
      case INTENT_PARAM:     fprintf(mFP, "<B>param</B> ");     break;
      case INTENT_TYPE:      fprintf(mFP, "<B>type</B> ");      break;
      case INTENT_BLANK:                                        break;
    }

    fprintf(mFP, "<B>arg</B> ");

    writeSymbol(node->sym, true);

  } else if (isLabelSymbol(node->sym)) {
    fprintf(mFP, "<B>label</B> ");
    writeSymbol(node->sym, true);

  } else if (isModuleSymbol(node->sym)) {
    fprintf(mFP, "%s\n", HTML_DL_close_tag);
    // Don't process nested modules -- they'll be handled at the top-level
    retval = false;

  } else {
    fprintf(mFP, "<B>def</B> ");
    writeSymbol(node->sym, true);
  }

  return retval;
}

void AstDumpToHtml::exitDefExpr(DefExpr* node) {
  if (isFnSymbol(node->sym) ||
      (isTypeSymbol(node->sym) &&
       isAggregateType(node->sym->type))) {

    fprintf(mFP, "</UL>\n");

    if (FnSymbol* fn = toFnSymbol(node->sym)) {
      fprintf(mFP, "<CHPLTAG=\"FN%d\">\n", fn->id);
    }

    fprintf(mFP, "</UL>\n");
  }

  if (isBlockStmt(node->parentExpr)) {
    fprintf(mFP, "%s\n", HTML_DL_close_tag);
  }
}


//
// NamedExpr
//
bool AstDumpToHtml::enterNamedExpr(NamedExpr* node) {
  if (isBlockStmt(node->parentExpr)) {
    fprintf(mFP, "%s\n", HTML_DL_open_tag);
  }

  fprintf(mFP, " (%s = ", node->name);

  return true;
}

void AstDumpToHtml::exitNamedExpr(NamedExpr* node) {
  fprintf(mFP, ")");

  if (isBlockStmt(node->parentExpr)) {
    fprintf(mFP, "%s\n", HTML_DL_close_tag);
  }
}


//
// IfExpr
//
bool AstDumpToHtml::enterIfExpr(IfExpr* node) {
  return false;
}

void AstDumpToHtml::exitIfExpr(IfExpr* node) {
}


//
// SymExpr
//
void AstDumpToHtml::visitSymExpr(SymExpr* node) {
  Symbol*    sym = node->symbol();
  VarSymbol* var = toVarSymbol(sym);

  if (isBlockStmt(node->parentExpr) == true) {
    fprintf(mFP, "%s\n", HTML_DL_open_tag);
  }

  fprintf(mFP, " ");

  if (var != 0 && var->immediate != 0) {
    const size_t bufSize = 128;
    char         imm[bufSize];

    snprint_imm(imm, bufSize, *var->immediate);

    fprintf(mFP, "<i><FONT COLOR=\"blue\">%s%s</FONT></i>", imm, is_imag_type(var->type) ? "i" : "");

  } else {
    writeSymbol(sym, false);
  }

  if (isBlockStmt(node->parentExpr)) {
    fprintf(mFP, "%s\n", HTML_DL_close_tag);
  }
}


//
// UnresolvedSymExpr
//
void AstDumpToHtml::visitUsymExpr(UnresolvedSymExpr* node) {
  if (isBlockStmt(node->parentExpr)) {
    fprintf(mFP, "%s\n", HTML_DL_open_tag);
  }

  fprintf(mFP, " <FONT COLOR=\"red\">%s</FONT>", node->unresolved);

  if (isBlockStmt(node->parentExpr)) {
    fprintf(mFP, "%s\n", HTML_DL_close_tag);
  }
}

//
// LoopExpr
//
bool AstDumpToHtml::enterLoopExpr(LoopExpr* node) {
  if (isBlockStmt(node->parentExpr)) {
    fprintf(mFP, "%s\n", HTML_DL_open_tag);
  }

  fprintf(mFP, "(%d ", node->id);

  if (node->forall) {
    if (node->maybeArrayType) fprintf(mFP, "[ ");
    else fprintf(mFP, "<B>forall</B> ");
  } else {
    fprintf(mFP, "<B>for</B> ");
  }

  if (node->indices) {
    node->indices->accept(this);
    fprintf(mFP, " <B>in</B> ");
  }

  if (node->zippered) fprintf(mFP, "<B>zip</B>");
  node->iteratorExpr->accept(this);

  if (node->maybeArrayType) fprintf(mFP, " <B>]</B>");
  else fprintf(mFP, " <B>do</B>");

  if (node->cond) {
    fprintf(mFP, " <B>if</B> ");
    node->cond->accept(this);
    fprintf(mFP, " <B>then</B> ");
  }

  node->loopBody->accept(this);

  fprintf(mFP, ")");

  return false;
}

void AstDumpToHtml::exitLoopExpr(LoopExpr* node) {
}


//
// UseStmt
//
void AstDumpToHtml::visitUseStmt(UseStmt* node) {
  if (isBlockStmt(node->parentExpr)) {
    fprintf(mFP, "%s\n", HTML_DL_open_tag);
  }

  fprintf(mFP, " (%d 'use' ", node->id);

  node->src->accept(this);

  if (node->isARename()) {
    fprintf(mFP, " 'as' %s", node->getRename());
  }

  if (!node->isPlainUse()) {
    node->writeListPredicate(mFP);
    bool first = outputVector(mFP, node->named);
    outputRenames(mFP, node->renamed, first);
  }

  fprintf(mFP, ")");

  if (isBlockStmt(node->parentExpr)) {
    fprintf(mFP, "%s\n", HTML_DL_close_tag);
  }
}

//
// ImportStmt
//
void AstDumpToHtml::visitImportStmt(ImportStmt* node) {
  if (isBlockStmt(node->parentExpr)) {
    fprintf(mFP, "%s\n", HTML_DL_open_tag);
  }

  fprintf(mFP, " (%d 'import' ", node->id);

  node->src->accept(this);

  fprintf(mFP, ")");

  if (isBlockStmt(node->parentExpr)) {
    fprintf(mFP, "%s\n", HTML_DL_close_tag);
  }
}


//
// BlockStmt
//
bool AstDumpToHtml::enterBlockStmt(BlockStmt* node) {
  fprintf(mFP, "%s\n", HTML_DL_open_tag);

  if (FnSymbol* fn = toFnSymbol(node->parentSymbol))
    if (node == fn->where)
      fprintf(mFP, "<B>where</B>\n");

  fprintf(mFP, "{");

  printBlockID(node);

  return true;
}

void AstDumpToHtml::exitBlockStmt(BlockStmt* node) {
  fprintf(mFP, "}");
  printBlockID(node);
  fprintf(mFP, "%s\n", HTML_DL_close_tag);
}


//
// WhileDoStmt
//
bool AstDumpToHtml::enterWhileDoStmt(WhileDoStmt* node) {
  fprintf(mFP, "%s\n", HTML_DL_open_tag);

  if (FnSymbol* fn = toFnSymbol(node->parentSymbol))
    if (node == fn->where)
      fprintf(mFP, "<B>where</B>\n");

  fprintf(mFP, "<B>WhileDo<B> {");

  printBlockID(node);

  return true;
}

void AstDumpToHtml::exitWhileDoStmt(WhileDoStmt* node) {
  fprintf(mFP, "}");
  printBlockID(node);
  fprintf(mFP, "%s\n", HTML_DL_close_tag);
}


//
// DoWhileStmt
//
bool AstDumpToHtml::enterDoWhileStmt(DoWhileStmt* node) {
  fprintf(mFP, "%s\n", HTML_DL_open_tag);

  if (FnSymbol* fn = toFnSymbol(node->parentSymbol))
    if (node == fn->where)
      fprintf(mFP, "<B>where</B>\n");

  fprintf(mFP, "<B>DoWhile<B> {");

  printBlockID(node);

  return true;
}

void AstDumpToHtml::exitDoWhileStmt(DoWhileStmt* node) {
  fprintf(mFP, "}");
  printBlockID(node);
  fprintf(mFP, "%s\n", HTML_DL_close_tag);
}


//
// ForLoop
//
bool AstDumpToHtml::enterForLoop(ForLoop* node) {
  fprintf(mFP, "%s\n", HTML_DL_open_tag);

  if (FnSymbol* fn = toFnSymbol(node->parentSymbol))
    if (node == fn->where)
      fprintf(mFP, "<B>where</B>\n");

  fprintf(mFP, "<B>ForLoop<B> {");

  printBlockID(node);

  return true;
}

void AstDumpToHtml::exitForLoop(ForLoop* node) {
  fprintf(mFP, "}");
  printBlockID(node);
  fprintf(mFP, "%s\n", HTML_DL_close_tag);
}


//
// CForLoop
//
bool AstDumpToHtml::enterCForLoop(CForLoop* node) {
  fprintf(mFP, "%s\n", HTML_DL_open_tag);

  if (FnSymbol* fn = toFnSymbol(node->parentSymbol))
    if (node == fn->where)
      fprintf(mFP, "<B>where</B>\n");

  fprintf(mFP, "<B>CForLoop<B> {");

  printBlockID(node);

  return true;
}

void AstDumpToHtml::exitCForLoop(CForLoop* node) {
  fprintf(mFP, "}");
  printBlockID(node);
  fprintf(mFP, "%s\n", HTML_DL_close_tag);
}


//
// ParamForLoop
//
bool AstDumpToHtml::enterParamForLoop(ParamForLoop* node) {
  fprintf(mFP, "%s\n", HTML_DL_open_tag);

  if (FnSymbol* fn = toFnSymbol(node->parentSymbol))
    if (node == fn->where)
      fprintf(mFP, "<B>where</B>\n");

  fprintf(mFP, "<B>ParamForLoop<B> {");

  printBlockID(node);

  return true;
}

void AstDumpToHtml::exitParamForLoop(ParamForLoop* node) {
  fprintf(mFP, "}");
  printBlockID(node);
  fprintf(mFP, "%s\n", HTML_DL_close_tag);
}


//
// CondStmt
//
bool AstDumpToHtml::enterCondStmt(CondStmt* node) {
  fprintf(mFP, "%s\n", HTML_DL_open_tag);
  fprintf(mFP, "<B>if</B> ");

  return true;
}

void AstDumpToHtml::exitCondStmt(CondStmt* node) {
  fprintf(mFP, "%s\n", HTML_DL_close_tag);
}

//
// ExternBlockStmt
//
void AstDumpToHtml::visitEblockStmt(ExternBlockStmt* node) {
  fprintf(mFP, "(%s", node->astTagAsString());

  if (isBlockStmt(node->parentExpr)) {
    fprintf(mFP, "%s\n", HTML_DL_close_tag);
  }
}


//
// GotoStmt
//
bool AstDumpToHtml::enterGotoStmt(GotoStmt* node) {
  fprintf(mFP, "%s\n", HTML_DL_open_tag);

  switch (node->gotoTag) {
    case GOTO_NORMAL:
      fprintf(mFP, "<B>goto</B> ");
      break;
    case GOTO_BREAK:
      fprintf(mFP, "<B>break</B> ");
      break;
    case GOTO_CONTINUE:
      fprintf(mFP, "<B>continue</B> ");
      break;
    case GOTO_RETURN:
      fprintf(mFP, "<B>gotoReturn</B> ");
      break;
    case GOTO_GETITER_END:
      fprintf(mFP, "<B>gotoGetiterEnd</B> ");
      break;
    case GOTO_ITER_RESUME:
      fprintf(mFP, "<B>gotoIterResume</B> ");
      break;
    case GOTO_ITER_END:
      fprintf(mFP, "<B>gotoIterEnd</B> ");
      break;
    case GOTO_ERROR_HANDLING:
      fprintf(mFP, "<B>gotoErrorHandling</B> ");
      break;
    case GOTO_BREAK_ERROR_HANDLING:
      fprintf(mFP, "<B>gotoBreakErrorHandling</B> ");
      break;
    case GOTO_ERROR_HANDLING_RETURN:
      fprintf(mFP, "<B>gotoErrorHandlingReturn</B> ");
      break;
  }

  if (SymExpr* label = toSymExpr(node->label))
    if (label->symbol() != gNil)
      writeSymbol(label->symbol(), true);

  return true;
}

void AstDumpToHtml::exitGotoStmt(GotoStmt* node) {
  fprintf(mFP, "%s\n", HTML_DL_close_tag);
}

//
// Helper functions
//

void AstDumpToHtml::writeFnSymbol(FnSymbol* fn) {
  bool first = true;

  if (fn->_this && fn->_this->defPoint) {
    writeSymbol(fn->_this->type->symbol, false);
    fprintf(mFP, " . ");
  }

  writeSymbol(fn, true);

  fprintf(mFP, " ( ");

  for_formals(formal, fn) {
    if (!first) {
      fprintf(mFP, " , ");
    } else {
      first = false;
    }

    writeSymbol(formal, true);
  }

  fprintf(mFP, " ) ");

  switch (fn->retTag) {
    case RET_VALUE:                                break;
    case RET_REF:   fprintf(mFP, "<b>ref</b> ");   break;
    case RET_CONST_REF:   fprintf(mFP, "<b>const ref</b> ");   break;
    case RET_PARAM: fprintf(mFP, "<b>param</b> "); break;
    case RET_TYPE:  fprintf(mFP, "<b>type</b> ");  break;
  }

  if (fn->retType && fn->retType->symbol) {
    fprintf(mFP, " : ");
    writeSymbol(fn->retType->symbol, false);
  }
}

void AstDumpToHtml::writeSymbol(Symbol* sym, bool def) {
  if (def) {
    fprintf(mFP, "<A NAME=\"SYM%d\">", sym->id);

  } else if (sym->defPoint && sym->defPoint->parentSymbol && sym->defPoint->getModule()) {
    INT_ASSERT(hasHref(sym));

    fprintf(mFP, "<A HREF=\"%s#SYM%d\">",
            html_file_name(sPassIndex, sym->defPoint->getModule()->name),
            sym->id);
  } else {
    INT_ASSERT(!hasHref(sym));

    fprintf(mFP, "<A>");
  }

  if (isFnSymbol(sym)) {
    fprintf(mFP, "<FONT COLOR=\"blue\">");

  } else if (isTypeSymbol(sym)) {
    fprintf(mFP, "<FONT COLOR=\"green\">");

  } else {
    fprintf(mFP, "<FONT COLOR=\"red\">");
  }

  fprintf(mFP, "%s", sym->name);
  fprintf(mFP, "</FONT>");
  fprintf(mFP, "<FONT COLOR=\"grey\">[%d]</FONT>", sym->id);
  fprintf(mFP, "</A>");

  if (def &&
      !toTypeSymbol(sym) &&
      sym->type &&
      sym->type->symbol &&
      sym->type != dtUnknown) {
    fprintf(mFP, ":");
    writeSymbol(sym->type->symbol, false);
  }
}

void AstDumpToHtml::adjacent_passes(Symbol* sym) {
  if (hasHref(sym)) {
    if (sPassIndex > 1)
      fprintf(mFP, "<A HREF=\"%s#SYM%d\">&laquo</A>",   html_file_name(sPassIndex - 1, sym), sym->id);

    if (true)
      fprintf(mFP, "<A HREF=\"%s#SYM%d\">&raquo</A>\n", html_file_name(sPassIndex + 1, sym), sym->id);
  }
}

void AstDumpToHtml::printBlockID(Expr* expr) {
  if (fdump_html_print_block_IDs)
    fprintf(mFP, " %d", expr->id);
}

const char* AstDumpToHtml::html_file_name(int passNum, Symbol* sym) {
  return html_file_name(passNum, sym->defPoint->getModule()->name);
}

const char* AstDumpToHtml::html_file_name(int passNum, const char* module) {
  return astr("pass", istr(passNum), "_module_", astr(module, ".html"));
}

bool AstDumpToHtml::hasHref(Symbol* sym) {
  return sym->defPoint               &&
         sym->defPoint->parentSymbol &&
         sym->defPoint->getModule();
}
