//
//    STMT.CPP -- Code generator (C back-end)
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

#include "genc.h"

//
//    GenC
//

// interface: codegen

  void GenC::codegenStmt(Expr *stmt) {
      setInfoLineno(stmt->linenum());
      setInfoFilename(stmt->fname());
      if (stmt->linenum() > 0) {
          if (printCppLineno) {
              writeStatement(
                  "/* ZLINE: " + numToString(stmt->linenum())
                      + " " + stmt->fname() + " */\n");
              } 
          }
      if (fGenIDS)
          writeStatement("/* " + numToString(stmt->id) + "*/ ");
      }

// overrides

  GenRet GenC::BlockStmt_codegen(BlockStmt *self) {
      Expr *parentExpr = self->parentExpr;
      AList *body = &self->body;
      CallExpr *blockInfo = self->blockInfo;

      GenRet ret;

      codegenStmt(self);

       if (blockInfo) {
          if (blockInfo->isPrimitive(PRIM_BLOCK_WHILEDO_LOOP)) {
              std::string hdr = 
                  "while (" + codegenValue(evalArg(blockInfo, 1)).c + ") ";
              writeStatement(hdr);
              }
          else if (blockInfo->isPrimitive(PRIM_BLOCK_DOWHILE_LOOP)) {
              writeStatement("do ");
              }
          else if (blockInfo->isPrimitive(PRIM_BLOCK_FOR_LOOP)) {
              std::string hdr = 
                  "for (;" + codegenValue(evalArg(blockInfo, 1)).c + ";) ";
              writeStatement(hdr);
              }
          else if (blockInfo->isPrimitive(PRIM_BLOCK_XMT_PRAGMA_FORALL_I_IN_N)) {
              std::string hdr = "_Pragma(\"mta for all streams ";
              hdr += codegenValue(evalArg(blockInfo, 1)).c;
              hdr += " of ";
              hdr += codegenValue(evalArg(blockInfo, 2)).c;
              hdr += "\")\n";
              writeStatement(hdr);
              }
          }

      if (self != self->getFunction()->body)
          writeStatement("{\n");

      if (!fNoRepositionDefExpr) {
          Vec<BaseAST*> asts;
          collect_top_asts(self, asts);
          forv_Vec(BaseAST, ast, asts) {
              if (DefExpr *def = toDefExpr(ast)) {
                  if (def->parentExpr == self) {
                      if (!toTypeSymbol(def->sym)) {
                          if (fGenIDS && isVarSymbol(def->sym))
                              writeStatement(
                                  "/* " + numToString(def->sym->id) + " */ ");
                              Symbol_codegenDef(def->sym);
                          }
                      }
                  }
              }
          }
 
      AList_codegen(body, "");

      if (blockInfo && blockInfo->isPrimitive(PRIM_BLOCK_DOWHILE_LOOP)) {
          std::string ftr = 
              "} while (" + codegenValue(evalArg(blockInfo, 1)).c + ");\n";
          writeStatement(ftr);
          }
      else if (self != self->getFunction()->body) {
          std::string end = "}";
          CondStmt *cond = toCondStmt(parentExpr);
          if (!cond || !(cond->thenStmt == self && cond->elseStmt))
              end += "\n";
          writeStatement(end);
          }
      return ret;
      }

  GenRet GenC::CondStmt_codegen(CondStmt *self) {
      Expr *condExpr = self->condExpr;
      Expr *thenStmt = self->thenStmt;
      Expr *elseStmt = self->elseStmt;

      GenRet ret;
      codegenStmt(self);
      writeStatement("if (" + codegenValue(eval(condExpr)).c + ") ");
      Expr_codegen(thenStmt);
      if (elseStmt) {
          writeStatement(" else ");
          Expr_codegen(elseStmt);
          }
      return ret;
      }

  GenRet GenC::GotoStmt_codegen(GotoStmt *self) {
      Expr *label = self->label;

      GenRet ret;
      codegenStmt(self);
      writeStatement("goto " + Expr_codegen(label).c + ";\n");
      return ret;
      }

  GenRet GenC::ExternBlockStmt_codegen(ExternBlockStmt *self) {
      GenRet ret;
    // Needs to be handled specially by creating a C
    //  file per module..
      INT_FATAL(self, "ExternBlockStmt codegen called");
      return ret;
      }

