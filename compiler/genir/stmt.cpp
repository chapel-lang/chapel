//
//    STMT.CPP -- Code generator (IR back-end)
//
//        Statements
//
//    Copyright (c) 2004-2013, Cray Inc.
//    Copyright (c) 2013, Unicorn Enterprises SA
//

#include <cstring>
#include "astutil.h"
#include "expr.h"
#include "files.h"
#include "misc.h"
#include "passes.h"
#include "stmt.h"
#include "stringutil.h"

#include "irbase.h"
#include "genir.h"

//
//    GenIR
//

// interface: codegen

  void GenIR::logStmt(Expr *stmt) {
      int lineno = stmt->linenum();
      const char *filename = stmt->fname();
      setInfoLineno(lineno);
      setInfoFilename(filename);
      ir->logLine(lineno, filename);
      ir->logId(stmt->id);
      }

// overrides

  GenRet GenIR::BlockStmt_codegen(BlockStmt *self) {
      GenRet ret;

      logStmt(self);

      CallExpr *blockInfo = self->blockInfo;
      if (blockInfo) {
          if (blockInfo->isPrimitive(PRIM_BLOCK_WHILEDO_LOOP)) {
              GenRet cond = codegenValue(evalArg(blockInfo, 1));
              ir->startWhiledo(cond.val);
              }
          else if (blockInfo->isPrimitive(PRIM_BLOCK_DOWHILE_LOOP)) {
              GenRet cond = codegenValue(evalArg(blockInfo, 1));
              ir->startDowhile(cond.val);
              }
          else if (blockInfo->isPrimitive(PRIM_BLOCK_FOR_LOOP)) {
              GenRet cond = codegenValue(evalArg(blockInfo, 1));
              ir->startFor(cond.val);
              }
          else if (blockInfo->isPrimitive(
                  PRIM_BLOCK_XMT_PRAGMA_FORALL_I_IN_N)) {
              GenRet a = codegenValue(evalArg(blockInfo, 1));
              GenRet b = codegenValue(evalArg(blockInfo, 2));
              ir->startXmtForall(a.val, b.val);
              }
          else    // cannot happen?
              ir->startBlock();
          }
      else
          ir->startBlock();

      if (!fNoRepositionDefExpr) {
          Vec<BaseAST*> asts;
          collect_top_asts(self, asts);
          forv_Vec(BaseAST, ast, asts) {
              if (DefExpr *def = toDefExpr(ast)) {
                  if (def->parentExpr == self) {
                      if (!toTypeSymbol(def->sym)) {
                          if (isVarSymbol(def->sym))
                              ir->logId(def->sym->id);
                          Symbol_codegenDef(def->sym);
                          }
                      }
                  }
              }
          }
 
       for_alist(node, self->body) {
          Expr_codegen(node);
          }

      if (blockInfo) {
          if (blockInfo->isPrimitive(PRIM_BLOCK_WHILEDO_LOOP))
              ir->endWhiledo();
          else if (blockInfo->isPrimitive(PRIM_BLOCK_DOWHILE_LOOP))
              ir->endDowhile();
          else if (blockInfo->isPrimitive(PRIM_BLOCK_FOR_LOOP))
              ir->endFor();
          else if (blockInfo->isPrimitive(
                  PRIM_BLOCK_XMT_PRAGMA_FORALL_I_IN_N))
              ir->endXmtForall();
          else    // cannot happen?
              ir->endBlock();
          }
      else
          ir->endBlock();

      return ret;
      }

  GenRet GenIR::CondStmt_codegen(CondStmt *self) {
      GenRet ret;
      GenRet cond = codegenValue(eval(self->condExpr));
      ir->startIf(cond.val);
      Expr_codegen(self->thenStmt);
      if (self->elseStmt) {
          ir->startElse();
          Expr_codegen(self->elseStmt);
          }
      ir->endIf();
      return ret;
      }

  GenRet GenIR::GotoStmt_codegen(GotoStmt *self) {
      GenRet ret;
      Expr *label = self->label;
      const char *cname;
      if (isDefExpr(label))
          cname = toDefExpr(label)->sym->cname;
      else
          cname = toSymExpr(label)->var->cname;
      ir->genGoto(cname);
      return ret;
      }

  GenRet GenIR::ExternBlockStmt_codegen(ExternBlockStmt *self) {
      GenRet ret;
    // Needs to be handled specially by creating a C
    //  file per module..
      INT_FATAL(self, "ExternBlockStmt codegen called");
      return ret;
      }

