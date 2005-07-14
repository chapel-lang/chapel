#include "insertLiteralTemps.h"
#include "symtab.h"
#include "symscope.h"
#include "symbol.h"
#include "type.h"
#include "expr.h"
#include "stmt.h"
#include "stringutil.h"


//
// This is a hack to handle some basic appends and prepends until
// where is implemented and # is overloaded in the sequence module.
//
static bool notSequenceType(Type* type) {
  return type == dtInteger || type == dtString;
}
static void handleBasicSequenceAppendPrependOperations(BinOp* binOp) {
  Type* leftType = binOp->left->typeInfo();
  Type* rightType = binOp->right->typeInfo();

  if (notSequenceType(leftType) && notSequenceType(rightType)) {
    INT_FATAL(binOp, "Bad # operation");
  }

  if (notSequenceType(leftType)) {
    binOp->replace(new ParenOpExpr(
                     new MemberAccess(binOp->right->copy(),
                       new UnresolvedSymbol("_prepend")),
                     new AList<Expr>(binOp->left->copy())));
  } else if (notSequenceType(rightType)) {
    binOp->replace(new ParenOpExpr(
                     new MemberAccess(binOp->left->copy(),
                       new UnresolvedSymbol("_append")),
                     new AList<Expr>(binOp->right->copy())));
  }
}


static void replaceSequenceLiteral(SeqExpr* seqExpr) {
  static int uid = 1;
  if (!analyzeAST) {
    INT_FATAL(seqExpr, "Analysis required for sequences");
  }
  char* name = glomstrings(2, "_seq_temp_", intstring(uid++));
  DefExpr* def = Symboltable::defineSingleVarDef(name,
                                                 dtUnknown,
                                                 NULL,
                                                 VAR_NORMAL,
                                                 VAR_VAR);
  Type* elt_type = dtUnknown;
  if (seqExpr->exprls->length() > 0) {
    elt_type = seqExpr->exprls->representative()->typeInfo();
    if (!elt_type->symbol) {
      elt_type = dtUnknown;
    }
  }
  def->sym->type =
    new ExprType(
      new ParenOpExpr(
        new Variable(
          new UnresolvedSymbol("seq")),
        new AList<Expr>(
          new Variable(elt_type->symbol))));

  seqExpr->getStmt()->insertBefore(new ExprStmt(def));

  Symbol* seq = def->sym;
  for_alist(Expr, tmp, seqExpr->exprls) {
    Expr* append =
      new ParenOpExpr(
        new MemberAccess(
          new Variable(seq),
          new UnresolvedSymbol("_append")),
        new AList<Expr>(tmp->copy()));
    seqExpr->getStmt()->insertBefore(new ExprStmt(append));
  }
  seqExpr->replace(new Variable(seq));
}


static void replaceArithmeticSequenceLiteral(SimpleSeqExpr* literal) {
  if (literal->parentExpr && dynamic_cast<ForallExpr*>(literal->parentExpr)) {
    return;
  }
  if (literal->parentExpr && dynamic_cast<Tuple*>(literal->parentExpr)) {
    return;
  }
  AList<Expr>* argList = new AList<Expr>();
  argList->insertAtTail(literal->lo->copy());
  argList->insertAtTail(literal->hi->copy());
  argList->insertAtTail(literal->str->copy());
  Expr* baseExpr = new Variable(new UnresolvedSymbol("_aseq"));
  literal->replace(new ParenOpExpr(baseExpr, argList));
}


static void replaceTupleLiteral(Tuple* tuple) {
  AList<Expr>* argList = new AList<Expr>();
  for_alist(Expr, expr, tuple->exprs) {
    argList->insertAtTail(new Variable(dtUnknown->symbol));
  }
  for_alist(Expr, expr, tuple->exprs) {
    argList->insertAtTail(expr->copy());
  }
  tuple->replace(
    new ParenOpExpr(
      new Variable(
        new UnresolvedSymbol(
          glomstrings(2, "_tuple", intstring(tuple->exprs->length())))),
      argList));
}


static void replaceTupleLiteral2(Tuple* tuple) {
  static int uid = 1;
  char* name = glomstrings(2, "_tuple_temp_", intstring(uid++));
  AList<Expr>* argList = new AList<Expr>();
  for_alist(Expr, expr, tuple->exprs) {
    argList->insertAtTail(new Variable(dtUnknown->symbol));
  }
  for_alist(Expr, expr, tuple->exprs) {
    argList->insertAtTail(expr->copy());
  }
  Expr* init = new ParenOpExpr(
                 new Variable(
                   new UnresolvedSymbol(
                     glomstrings(2, "_tuple", intstring(tuple->exprs->length())))),
                 argList);
  DefExpr* def = Symboltable::defineSingleVarDef(name,
                                                 dtUnknown,
                                                 init,
                                                 VAR_NORMAL,
                                                 VAR_VAR);
  tuple->getStmt()->insertBefore(new ExprStmt(def));
  Symbol* tmp = def->sym;
  int i = 1;
  for_alist(Expr, expr, tuple->exprs) {
    AssignOp* assignOp = new AssignOp(GETS_NORM, expr->copy(),
                                      new ParenOpExpr(
                                        new Variable(tmp),
                                        new AList<Expr>(
                                          new IntLiteral(intstring(i), i))));
    tuple->getStmt()->insertAfter(new ExprStmt(assignOp));
    i++;
  }
  tuple->replace(new Variable(tmp));
}


static void replaceComplexLiteral(ComplexLiteral* complexLiteral) {
  AList<Expr>* argList = new AList<Expr>();
  FloatLiteral* realPart = new FloatLiteral(complexLiteral->realStr,
                                            complexLiteral->realVal);
  argList->insertAtTail(realPart);
  // remove i, why is this here anyway?  --SJD
  complexLiteral->str[strlen(complexLiteral->str)-1] = '\0';
  FloatLiteral* imagPart = new FloatLiteral(complexLiteral->str,
                                            complexLiteral->imagVal);
  argList->insertAtTail(imagPart);
  complexLiteral->replace(new ParenOpExpr(new Variable(new UnresolvedSymbol("complex")), argList));
}


static void createTupleBaseType(int size) {
  char *name = glomstrings(2, "_tuple", intstring(size));
  if (Symboltable::lookupInScope(name, commonModule->modScope)) {
    return;
  }
  SymScope* saveScope = Symboltable::setCurrentScope(commonModule->modScope);
  Symboltable::pushScope(SCOPE_CLASS);
  AList<Stmt>* decls = new AList<Stmt>();
  Vec<TypeSymbol*> types;
  Vec<VarSymbol*> fields;
  for (int i = 1; i <= size; i++) {
    VariableType* variableType = new VariableType(getMetaType(0));
    TypeSymbol* typeSymbol =
      new TypeSymbol(glomstrings(2, "_t", intstring(i)), variableType);
    variableType->addSymbol(typeSymbol);
    decls->insertAtTail(new ExprStmt(new DefExpr(typeSymbol)));
    types.add(typeSymbol);
  }
  for (int i = 1; i <= size; i++) {
    DefExpr* def = 
      Symboltable::defineSingleVarDef(glomstrings(2, "_f", intstring(i)),
                                      types.v[i-1]->type,
                                      NULL,
                                      VAR_NORMAL,
                                      VAR_VAR);
    decls->insertAtTail(new ExprStmt(def));
    VarSymbol* var = dynamic_cast<VarSymbol*>(def->sym);
    fields.add(var);
  }
  for (int i = 1; i <= size; i++) {
    FnSymbol* fn = Symboltable::startFnDef(new FnSymbol("this"));
    ParamSymbol* paramSymbol =
      new ParamSymbol(PARAM_PARAMETER, "index", dtInteger);
    AList<DefExpr>* formals = new AList<DefExpr>(new DefExpr(paramSymbol));
    Expr* where = new BinOp(BINOP_EQUAL, new Variable(paramSymbol),
                            new IntLiteral(intstring(i), i));
    Symboltable::continueFnDef(fn, formals, dtUnknown, true, where);
    AList<Stmt>* body = new AList<Stmt>(new ReturnStmt(new Variable(fields.v[i-1])));
    Symboltable::finishFnDef(fn, new BlockStmt(body));
    decls->insertAtTail(new ExprStmt(new DefExpr(fn)));
  }
  SymScope *scope = Symboltable::popScope();
  Type* type = Symboltable::defineStructType(name, new RecordType(), scope, decls);
  commonModule->stmts->insertAtHead(new ExprStmt(type->symbol->defPoint));

  FnSymbol* fn = Symboltable::startFnDef(new FnSymbol("write"));
  Vec<char*>* uname = new Vec<char*>();
  uname->add(glomstrings(2, "_tuple", intstring(size)));
  UnresolvedType* utype = new UnresolvedType(uname);
  ParamSymbol* paramSymbol =
    new ParamSymbol(PARAM_BLANK, "val", utype);
  AList<DefExpr>* formals = new AList<DefExpr>(new DefExpr(paramSymbol));
  Symboltable::continueFnDef(fn, formals, dtUnknown, false, NULL);
  AList<Expr>* args = new AList<Expr>();
  args->insertAtTail(new StringLiteral(copystring("(")));
  for (int i = 1; i <= size; i++) {
    if (i != 1) {
      args->insertAtTail(new StringLiteral(copystring(", ")));
    }
    args->insertAtTail(new MemberAccess(new Variable(new UnresolvedSymbol("val")),
                                        new UnresolvedSymbol(glomstrings(2, "_f", intstring(i)))));
  }
  args->insertAtTail(new StringLiteral(copystring(")")));
  Stmt* writeBody =
    new ExprStmt(
      new ParenOpExpr(
        new Variable(
          new UnresolvedSymbol("write")),
        args));
  AList<Stmt>* body = new AList<Stmt>(writeBody);
  Symboltable::finishFnDef(fn, new BlockStmt(body));
  commonModule->stmts->insertAtTail(new ExprStmt(new DefExpr(fn)));
  Symboltable::setCurrentScope(saveScope);
}


void InsertLiteralTemps::postProcessExpr(Expr* expr) {
  if (SimpleSeqExpr* literal = dynamic_cast<SimpleSeqExpr*>(expr)) {
    replaceArithmeticSequenceLiteral(literal);
  } else if (SeqExpr* literal = dynamic_cast<SeqExpr*>(expr)) {
    replaceSequenceLiteral(literal);
  } else if (ComplexLiteral* literal = dynamic_cast<ComplexLiteral*>(expr)) {
    replaceComplexLiteral(literal);
  } else if (BinOp* binOp = dynamic_cast<BinOp*>(expr)) {
    if (binOp->type == BINOP_SEQCAT) {
      handleBasicSequenceAppendPrependOperations(binOp);
    }
  } else if (Tuple* literal = dynamic_cast<Tuple*>(expr)) {
    createTupleBaseType(literal->exprs->length());
    if (dynamic_cast<ForLoopStmt*>(literal->parentStmt)) {
      return;  // HACK for domains
    }
    if (AssignOp* assignOp = dynamic_cast<AssignOp*>(literal->parentExpr)) {
      if (assignOp->left == literal) {
        replaceTupleLiteral2(literal); // Handle destructuring
        return;
      }
    }
    replaceTupleLiteral(literal);
  }
}


void InsertLiteralTemps::postProcessStmt(Stmt* stmt) {

}


void InsertLiteralTemps::postProcessType(Type* type) {
  if (ExprType* exprType = dynamic_cast<ExprType*>(type)) {
    if (ParenOpExpr* parenOpExpr = dynamic_cast<ParenOpExpr*>(exprType->expr)) {
      if (Variable* variable = dynamic_cast<Variable*>(parenOpExpr->baseExpr)) {
        if (!strncmp(variable->var->name, "_tuple", 6)) {
          createTupleBaseType(parenOpExpr->argList->length());
        }
      }
    }
  }
}
