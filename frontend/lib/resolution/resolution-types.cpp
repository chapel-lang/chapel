/*
 * Copyright 2021-2025 Hewlett Packard Enterprise Development LP
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

#include "chpl/resolution/can-pass.h"
#include "chpl/resolution/resolution-types.h"

#include "chpl/parsing/parsing-queries.h"
#include "chpl/framework/global-strings.h"
#include "chpl/framework/query-impl.h"
#include "chpl/framework/update-functions.h"
#include "chpl/resolution/resolution-queries.h"
#include "chpl/resolution/scope-queries.h"
#include "chpl/types/EnumType.h"
#include "chpl/types/TupleType.h"
#include "chpl/uast/Builder.h"
#include "chpl/uast/FnCall.h"
#include "chpl/uast/OpCall.h"
#include "chpl/uast/Formal.h"
#include "chpl/uast/Identifier.h"
#include "chpl/uast/For.h"
#include "chpl/uast/VarArgFormal.h"
#include "chpl/util/clang-integration.h"

#include "Resolver.h"
#include "extern-blocks.h"

#include <iomanip>

namespace chpl {
namespace resolution {

using namespace uast;
using namespace types;

const owned<UntypedFnSignature>&
UntypedFnSignature::getUntypedFnSignature(Context* context, ID id,
                                          UniqueString name,
                                          bool isMethod,
                                          bool isTypeConstructor,
                                          bool isCompilerGenerated,
                                          bool throws,
                                          asttags::AstTag idTag,
                                          uast::Function::Kind kind,
                                          std::vector<FormalDetail> formals,
                                          const AstNode* whereClause,
                                          ID compilerGeneratedOrigin) {
  QUERY_BEGIN(getUntypedFnSignature, context,
              id, name, isMethod, isTypeConstructor, isCompilerGenerated,
               throws, idTag, kind, formals, whereClause, compilerGeneratedOrigin);

  owned<UntypedFnSignature> result =
    toOwned(new UntypedFnSignature(id, name,
                                   isMethod, isTypeConstructor,
                                   isCompilerGenerated, throws, idTag, kind,
                                   std::move(formals), whereClause,
                                   compilerGeneratedOrigin));

  return QUERY_END(result);
}

const UntypedFnSignature*
UntypedFnSignature::get(Context* context, ID id,
                        UniqueString name,
                        bool isMethod,
                        bool isTypeConstructor,
                        bool isCompilerGenerated,
                        bool throws,
                        asttags::AstTag idTag,
                        uast::Function::Kind kind,
                        std::vector<FormalDetail> formals,
                        const uast::AstNode* whereClause,
                        ID compilerGeneratedOrigin) {
  return getUntypedFnSignature(context, id, name,
                               isMethod, isTypeConstructor,
                               isCompilerGenerated, throws, idTag, kind,
                               std::move(formals), whereClause,
                               compilerGeneratedOrigin).get();
}

const UntypedFnSignature*
getUntypedFnSignatureForFn(Context* context, const uast::Function* fn, ID const* compilerGeneratedOrigin) {
  const UntypedFnSignature* result = nullptr;

  if (fn != nullptr) {
    // compute the FormalDetails
    std::vector<UntypedFnSignature::FormalDetail> formals;
    for (auto decl : fn->formals()) {
      UniqueString name;
      bool hasDefault = false;
      if (auto formal = decl->toFormal()) {
        name = formal->name();
        hasDefault = formal->initExpression() != nullptr;
      } else if (auto formal = decl->toTupleDecl()) {
        hasDefault = formal->initExpression() != nullptr;
      } else if (auto varargs = decl->toVarArgFormal()) {
        name = varargs->name();

        // This should not be possible. Currently varargs with a default value
        // will be considered a syntax error.
        hasDefault = false;
        CHPL_ASSERT(varargs->initExpression() == nullptr);
      }

      auto defaultKind = hasDefault ? UntypedFnSignature::DK_DEFAULT
                                    : UntypedFnSignature::DK_NO_DEFAULT;
      auto fd = UntypedFnSignature::FormalDetail(name, defaultKind,
                                                 decl, decl->isVarArgFormal());
      formals.push_back(fd);
    }

    // find the unique-ified untyped signature
    result = UntypedFnSignature::get(context, fn->id(), fn->name(),
                                     fn->isMethod(),
                                     /* isTypeConstructor */ false,
                                     /* isCompilerGenerated */ compilerGeneratedOrigin != nullptr,
                                     /* throws */ fn->throws(),
                                     /* idTag */ asttags::Function,
                                     fn->kind(),
                                     std::move(formals), fn->whereClause(),
                                     compilerGeneratedOrigin ? *compilerGeneratedOrigin
                                                             : ID());
  }

  return result;
}

const UntypedFnSignature* UntypedFnSignature::get(Context* context,
                                                  const Function* function) {
  if (function == nullptr) {
    return nullptr;
  }

  return getUntypedFnSignatureForFn(context, function);
}

static const UntypedFnSignature*
getUntypedFnSignatureForExternId(Context* context, ID functionId) {
  CHPL_ASSERT(functionId.isExternBlockElement());

  auto name = functionId.symbolName(context);
  auto externBlockId = functionId.parentSymbolId(context);

  auto tfs = externBlockSigForFn(context, externBlockId, name);

  return tfs->untyped();
}

static const UntypedFnSignature* const&
getUntypedFnSignatureForIdQuery(Context* context, ID functionId) {
  QUERY_BEGIN(getUntypedFnSignatureForIdQuery, context, functionId);

  const UntypedFnSignature* result = nullptr;
  const AstNode* ast = parsing::idToAst(context, functionId);

  if (ast != nullptr && ast->isFunction()) {
    result = getUntypedFnSignatureForFn(context, ast->toFunction());
  } else if (functionId.isExternBlockElement()) {
    result = getUntypedFnSignatureForExternId(context, functionId);
  }

  return QUERY_END(result);
}

const UntypedFnSignature* UntypedFnSignature::get(Context* context,
                                                  ID functionId) {
  if (functionId.isEmpty()) {
    return nullptr;
  }

  return getUntypedFnSignatureForIdQuery(context, functionId);
}

static UniqueString getCallName(const uast::Call* call) {
  UniqueString name;
  // Get the name of the called expression.
  if (auto op = call->toOpCall()) {
    name = op->op();
  } else if (auto called = call->calledExpression()) {
    if (auto calledIdent = called->toIdentifier()) {
      name = calledIdent->name();
    } else if (auto calledDot = called->toDot()) {
      name = calledDot->field();
    } else if (auto op = called->toOpCall()) {
      name = op->op();
    }
  }
  return name;
}

CallInfo CallInfo::createSimple(UniqueString calledFnName) {
  return CallInfo(calledFnName,
                  /* calledType */ types::QualifiedType(),
                  /* isMethodCall */ false,
                  /* hasQuestionArg */ false,
                  /* isParenless */ false,
                  { });
}

CallInfo CallInfo::createSimple(UniqueString calledFnName,
                                QualifiedType arg1type) {
  return CallInfo(calledFnName,
                  /* calledType */ types::QualifiedType(),
                  /* isMethodCall */ false,
                  /* hasQuestionArg */ false,
                  /* isParenless */ false,
                  {CallInfoActual(arg1type)});
}

CallInfo CallInfo::createSimple(UniqueString calledFnName,
                                QualifiedType arg1type,
                                QualifiedType arg2type) {
  return CallInfo(calledFnName,
                  /* calledType */ types::QualifiedType(),
                  /* isMethodCall */ false,
                  /* hasQuestionArg */ false,
                  /* isParenless */ false,
                  {CallInfoActual(arg1type), CallInfoActual(arg2type)});
}

CallInfo CallInfo::createUnknown(const uast::FnCall* call) {
  // Pieces of the CallInfo we need to prepare.
  UniqueString name;
  bool isMethodCall = false;
  bool hasQuestionArg = false;
  std::vector<CallInfoActual> actuals;

  // set the name (simple cases only)
  name = getCallName(call);

  int i = 0;
  for (auto actual : call->actuals()) {
    if (isQuestionMark(actual)) {
      hasQuestionArg = true;
    } else {
      UniqueString byName;
      if (call->isNamedActual(i)) {
        byName = call->actualName(i);
      }
      actuals.push_back(CallInfoActual(QualifiedType(), byName));
      i++;
    }
  }

  auto ret = CallInfo(name, QualifiedType(), isMethodCall, hasQuestionArg,
                      /* isParenless */ false, actuals);

  return ret;
}

void CallInfo::prepareActuals(Context* context,
                              const Call* call,
                              const ResolutionResultByPostorderID& byPostorder,
                              bool raiseErrors,
                              std::vector<CallInfoActual>& actuals,
                              const AstNode*& questionArg,
                              std::vector<const uast::AstNode*>* actualAsts) {
  // Prepare the actuals of the call.
  for (int i = 0; i < call->numActuals(); i++) {
    auto actual = call->actual(i);
    prepareActual(context, call, actual, i, byPostorder, raiseErrors,
                  actuals, questionArg, actualAsts);
  }
}

void CallInfo::prepareActual(Context* context,
                             const Call* call,
                             const AstNode* actual,
                             int actualIdx,
                             const ResolutionResultByPostorderID& byPostorder,
                             bool raiseErrors,
                             std::vector<CallInfoActual>& actuals,
                             const AstNode*& questionArg,
                             std::vector<const uast::AstNode*>* actualAsts) {
  auto fnCall = call ? call->toFnCall() : nullptr;

  if (isQuestionMark(actual)) {
    if (call && call->isTuple()) {
      // Expressions like (?,?,?)
      UniqueString byName;
      auto qt = QualifiedType(QualifiedType::TYPE, AnyType::get(context));
      actuals.push_back(CallInfoActual(qt, byName));
      if (actualAsts != nullptr) {
        actualAsts->push_back(actual);
      }
    } else if (questionArg == nullptr) {
      questionArg = actual;
    } else {
      CHPL_REPORT(context, MultipleQuestionArgs, fnCall, questionArg, actual);
      // Keep questionArg pointing at the first question argument we found
    }
  } else {
    QualifiedType actualType;
    // replace default value with resolved if available
    if (const ResolvedExpression* r = byPostorder.byAstOrNull(actual)) {
      actualType = r->type();
    }
    UniqueString byName;
    if (fnCall && fnCall->isNamedActual(actualIdx)) {
      byName = fnCall->actualName(actualIdx);
    }

    bool handledTupleExpansion = false;
    if (auto op = actual->toOpCall()) {
      if (op->op() == USTR("...")) {
        if (op->numActuals() != 1) {
          if (raiseErrors) {
            context->error(op, "tuple expansion can only accept one argument");
          }
          actualType = QualifiedType(QualifiedType::VAR,
              ErroneousType::get(context));
        } else {
          const ResolvedExpression& rr = byPostorder.byAst(op->actual(0));
          actualType = rr.type();
        }

        // handle tuple expansion
        if (!actualType.hasTypePtr() ||
            actualType.type()->isUnknownType()) {
          // leave the result unknown
          actualType = QualifiedType(QualifiedType::VAR,
              UnknownType::get(context));
        } else if (actualType.type()->isErroneousType()) {
          // let it stay erroneous type
        } else if (!actualType.type()->isTupleType()) {
          if (raiseErrors) {
            CHPL_REPORT(context, TupleExpansionNonTuple, fnCall, op, actualType);
          }
          actualType = QualifiedType(QualifiedType::VAR,
              ErroneousType::get(context));
        } else {
          if (!byName.isEmpty()) {
            CHPL_REPORT(context, TupleExpansionNamedArgs, op, fnCall);
          }

          auto tupleType = actualType.type()->toTupleType();
          int n = tupleType->numElements();
          for (int i = 0; i < n; i++) {
            tupleType->elementType(i);
            // intentionally use the empty name (to ignore it if it was
            // set and we issued an error above)
            actuals.push_back(CallInfoActual(tupleType->elementType(i),
                  UniqueString()));
            if (actualAsts != nullptr) {
              actualAsts->push_back(op);
            }
          }
          handledTupleExpansion = true;
        }
      }
    }

    if (!handledTupleExpansion) {
      actuals.push_back(CallInfoActual(actualType, byName));
      if (actualAsts != nullptr) {
        actualAsts->push_back(actual);
      }
    }
  }

}

// returns true if values of this kind should be treated as 'functional':
// if a call `x(args)` is being resolved, and `x` is a value, then
// we should resolve `x.this(args)` instead.
static bool isKindForFunctionalValue(QualifiedType::Kind kind) {
  return kind != QualifiedType::UNKNOWN &&
         kind != QualifiedType::TYPE &&
         kind != QualifiedType::FUNCTION;
}

// Returns true if, given a called dot's receiver's kind, that kind
// indicates the receiver should be treated as a method receiver. I.e.,
// if we have `x.f(args)`, returns true if the above should be treated
// as a method call with `f(this = x, args)`.
static bool isKindForMethodReceiver(QualifiedType::Kind kind) {
  return kind != QualifiedType::FUNCTION &&
         kind != QualifiedType::MODULE;
}

static optional<QualifiedType>
tryGetType(const AstNode* node, const ResolutionResultByPostorderID& byPostorder) {
  if (node) {
    if (auto r = byPostorder.byAstOrNull(node)) {
      return r->type();
    }
  }

  return {};
}

static QualifiedType convertToInitReceiverType(const QualifiedType original) {
  if (original.kind() != QualifiedType::TYPE &&
      original.kind() != QualifiedType::PARAM) {
    return QualifiedType(QualifiedType::INIT_RECEIVER, original.type());
  }
  return original;
}

CallInfo CallInfo::create(Context* context,
                          const Call* call,
                          const ResolutionResultByPostorderID& byPostorder,
                          bool raiseErrors,
                          std::vector<const uast::AstNode*>* actualAsts,
                          ID* moduleScopeId,
                          UniqueString rename) {

  // Pieces of the CallInfo we need to prepare.
  UniqueString name;
  QualifiedType calledType;
  bool isMethodCall = false;
  const AstNode* questionArg = nullptr;
  std::vector<CallInfoActual> actuals;

  if (actualAsts != nullptr) {
    actualAsts->clear();
  }

  name = getCallName(call);

  // Set up a method call if relevant.
  if (auto calledExpr = call->calledExpression()) {
    // It shouldn't be possible to have definitions that could match either a
    // normal method call or a call to 'this' on a field, so no need to
    // disambiguate here; assume it'll be one or the other.

    const AstNode* dotReceiver = nullptr;
    if (auto dot = calledExpr->toDot()) dotReceiver = dot->receiver();
    auto calledExprType = tryGetType(calledExpr, byPostorder);
    auto dotReceiverType = tryGetType(dotReceiver, byPostorder);

    auto makeCallToDotThis = [&]() {
      name = USTR("this");
      // add the 'this' argument as well
      isMethodCall = true;
      actuals.push_back(CallInfoActual(*calledExprType, USTR("this")));
      if (actualAsts != nullptr) {
        actualAsts->push_back(calledExpr);
      }
    };

    if (calledExprType &&
        (isKindForFunctionalValue(calledExprType->kind()) ||
         calledExprType->isErroneousType())) {
      // If e.g. x is a value (and not a function, then x(0) translates to x.this(0)
      // Run this case even if the receiver is a module, since we might be
      // trying to invoke 'this' on value x in M.x.
      //
      // In the case of ErroneousType, assume that the called thing was
      // a value (ambiguity or other "benign" UNKNOWN would not produce errors).
      // Later, this can lead to skipping resolving the call altogether.

      makeCallToDotThis();
    } else if (dotReceiverType && dotReceiverType->kind() == QualifiedType::MODULE) {
      // In calls like `M.f()`, where `M` is a module, we need to restrict
      // our search to `M`'s scope. Signal this by setting `moduleScopeId`.
      if (moduleScopeId != nullptr)
        *moduleScopeId = byPostorder.byAst(dotReceiver).toId();
      /* TODO: set calledType? */
    } else if (calledExprType && !calledExprType->isUnknown() && calledExprType->isType()) {
      // normally, we would say this is a type constructor and set calledType.
      // However, for tuples, we treat this as a regular "proc this", which
      // enables accessing the individual element's types.

      if (calledExprType->type() && calledExprType->type()->isTupleType()) {
        makeCallToDotThis();
      } else {
        calledType = *calledExprType;
      }
    } else if (!call->isOpCall() && dotReceiverType &&
               isKindForMethodReceiver(dotReceiverType->kind())) {
      // Check for normal method call, maybe construct a receiver.

      // If this is a receiver to 'init', adjust the receiver type to
      // use the INIT_RECEIVER intent.
      auto dotReceiverQt = *dotReceiverType;
      if (name == USTR("init")) {
        dotReceiverQt = convertToInitReceiverType(dotReceiverQt);
      }

      actuals.push_back(CallInfoActual(dotReceiverQt, USTR("this")));
      if (actualAsts != nullptr) {
        actualAsts->push_back(dotReceiver);
      }
      isMethodCall = true;
    }
  }

  // Prepare the remaining actuals.
  prepareActuals(context, call, byPostorder, raiseErrors,
                 actuals, questionArg, actualAsts);

  if (actualAsts != nullptr) {
    CHPL_ASSERT(actualAsts->size() == actuals.size());
  }

  if (!rename.isEmpty()) {
    // Whatever we were calling was a value, and is now and actual. Can't
    // rename an argument to a function...
    CHPL_ASSERT(name != "this");
    name = rename;
  }

  auto ret = CallInfo(name, calledType, isMethodCall,
                      /* hasQuestionArg */ questionArg != nullptr,
                      /* isParenless */ false, actuals);

  return ret;
}

CallInfo CallInfo::createWithReceiver(const CallInfo& ci,
                                      QualifiedType receiverType,
                                      UniqueString rename) {
  std::vector<CallInfoActual> newActuals;
  newActuals.push_back(CallInfoActual(receiverType, USTR("this")));

  // Replace existing 'this' in 'ci'
  int off = 0;
  if (ci.isMethodCall() && ci.actual(0).byName() == "this") {
    off = 1;
  }

  // append the other actuals
  newActuals.insert(newActuals.end(), ci.actuals_.begin() + off, ci.actuals_.end());

  auto name = rename.isEmpty() ? ci.name_ : rename;
  return CallInfo(name, QualifiedType(),
                  /* isMethodCall */ true,
                  ci.hasQuestionArg_,
                  ci.isParenless_,
                  std::move(newActuals));
}

CallInfo CallInfo::copyAndRename(const CallInfo &ci, UniqueString rename) {
  return CallInfo(rename, ci.calledType(), ci.isMethodCall(),
                  ci.hasQuestionArg_, ci.isParenless_, ci.actuals_);
}

void ResolutionResultByPostorderID::setupForSymbol(const AstNode* ast) {
  CHPL_ASSERT(Builder::astTagIndicatesNewIdScope(ast->tag()));

  symbolId_ = ast->id();
}
void ResolutionResultByPostorderID::setupForSignature(const Function* func) {
  symbolId_ = func->id();
}
void ResolutionResultByPostorderID::setupForParamLoop(
    const For* loop, ResolutionResultByPostorderID& parent) {
  this->symbolId_ = parent.symbolId_;
}
void ResolutionResultByPostorderID::setupForFunction(const Function* func) {
  setupForSymbol(func);
}

bool ResolutionResultByPostorderID::update(ResolutionResultByPostorderID& keep,
                                           ResolutionResultByPostorderID& addin)
{
  return defaultUpdate(keep, addin);
}

bool FormalActualMap::computeAlignment(const UntypedFnSignature* untyped,
                                       const TypedFnSignature* typed,
                                       const CallInfo& call) {

  // untyped must be provided but typed can be nullptr.
  CHPL_ASSERT(untyped);

  // create the mapping handling named and default arguments

  // TODO are these reused??
  // clear the current mapping
  byFormalIdx_.clear();
  actualIdxToFormalIdx_.clear();
  mappingIsValid_ = false;
  failingActualIdx_ = -1;

  int numFormals = untyped->numFormals();
  int numEntries = numFormals;

  // The specified number of varargs in the formal isn't known at this point,
  // so we need to build the entries in terms of the number of varargs that the
  // caller is attempting to pass.
  int attemptedNumVarArgs = 0;

  // allocate space in the arrays
  byFormalIdx_.resize(numEntries);
  actualIdxToFormalIdx_.resize(call.numActuals());

  // initialize the FormalActual parts from the Formals
  int entryIdx = 0;
  int numVarArgFormals = 0;
  for (int i = 0; i < numFormals; i++) {

    const Decl* decl = untyped->formalDecl(i);
    QualifiedType formalQT;
    bool formalInstantiated = false;

    if (typed != nullptr) {
      formalQT = typed->formalType(i);
      if (typed->instantiatedFrom() != nullptr) {
        formalInstantiated = typed->formalIsInstantiated(i);
      }
    }

    if (untyped->formalIsVarArgs(i) == false) {
      FormalActual& entry = byFormalIdx_[entryIdx];

      entry.formal_ = decl;
      entry.hasActual_ = false;
      entry.formalIdx_ = i;
      entry.actualIdx_ = -1;
      entry.formalType_ = formalQT;
      entry.formalInstantiated_ = formalInstantiated;
      entry.hasDefault_ = untyped->formalMightHaveDefault(i);

      entryIdx++;
    } else {
      // handle a varargs formal
      numVarArgFormals += 1;

      if (numVarArgFormals > 1) {
        return false;
      }

      // zero-sized varargs not currently supported
      int numExtra = call.numActuals() - numFormals;
      attemptedNumVarArgs = std::max(numExtra + 1, 1);
      numEntries = numFormals + attemptedNumVarArgs - 1;
      byFormalIdx_.resize(numEntries);

      QualifiedType starQT;
      if (!formalQT.isUnknownOrErroneous()) {
        const TupleType* tup = formalQT.type()->toTupleType();
        CHPL_ASSERT(tup);
        if (tup->isStarTuple()) {
          starQT = tup->starType();
        }
      }

      for (int j = 0; j < attemptedNumVarArgs; j++) {
        FormalActual& entry = byFormalIdx_[entryIdx];

        QualifiedType qt;
        if (starQT.type() != nullptr) {
          qt = starQT;
        } else if (!formalQT.isUnknownOrErroneous()) {
          // try to pull the type out of the formalQT if it
          // is after instantiation.
          const TupleType* tup = formalQT.type()->toTupleType();
          CHPL_ASSERT(tup);
          qt = tup->elementType(j);
        }

        // if the formal has concrete intent (e.g. 'out' or 'in' or 'ref'),
        // use the intent of the formal for the intent of the actual->formal
        // mapping entry.
        if (!formalQT.isNonConcreteIntent()) {
          qt = QualifiedType(formalQT.kind(), qt.type(), qt.param());
        }

        entry.formal_ = decl;
        entry.hasActual_ = false;
        entry.formalIdx_ = i;
        entry.actualIdx_ = -1;
        entry.formalType_ = qt;
        entry.formalInstantiated_ = formalInstantiated;
        entry.hasDefault_ = untyped->formalMightHaveDefault(i);
        entry.isVarArgEntry_ = true;

        entryIdx++;
      }
    }
  }

  CHPL_ASSERT(entryIdx == numEntries);

  // Match named actuals against formal names in the function signature.
  // Record successful matches in actualIdxToFormalIdx.

  // TODO: This should just be a string to int map...
  for (size_t actualIdx = 0; actualIdx < call.numActuals(); actualIdx++) {
    const CallInfoActual& actual = call.actual(actualIdx);
    if (!actual.byName().isEmpty()) {
      bool match = false;
      int entryIdx = 0;
      for (int i = 0; i < numFormals; i++) {
        FormalActual& entry = byFormalIdx_[entryIdx];
        match = actual.byName() == untyped->formalName(i);
        CHPL_ASSERT(entry.formal_ == untyped->formalDecl(i));

        if (entry.isVarArgEntry_) {
          // TODO: production compiler doesn't support named VarArgs, but
          // that appears to be an implementation limitation...
          if (match) {
            failingActualIdx_ = actualIdx;
            return false;
          }
          // Skip the other VarArgs
          entryIdx += attemptedNumVarArgs;
        } else {
          if (match) {
            entry.hasActual_ = true;
            entry.actualIdx_ = actualIdx;
            entry.actualType_ = actual.type();
            actualIdxToFormalIdx_[actualIdx] = entryIdx;
            break;
          }

          entryIdx++;
        }
      }

      // Fail if no matching formal is found.
      if (match == false) {
        failingActualIdx_ = actualIdx;
        // TODO: track failure type for error messages
        return false;
      }
    }
  }

  // Fill in unmatched formals in sequence with the remaining actuals.
  // Record successful substitutions
  entryIdx = 0;
  int actualIdx = 0;
  for (const CallInfoActual& actual : call.actuals()) {
    if (entryIdx >= numEntries) {
      // too many actuals
      failingActualIdx_ = actualIdx;
      return false;
    }

    if (actual.byName().isEmpty()) {
      // Skip any formals already matched to named arguments
      while (byFormalIdx_[entryIdx].hasActual_) {
        if (entryIdx + 1 >= numEntries) {
          // too many actuals
          failingActualIdx_ = actualIdx;
          return false;
        }
        entryIdx++;
      }
      CHPL_ASSERT(entryIdx < numEntries);

      // if this is a call to an operator method, skip the 'this' formal
      if (call.isOpCall() && untyped->formalName(entryIdx) == USTR("this")) {
        entryIdx++;
      }

      FormalActual& entry = byFormalIdx_[entryIdx];
      entry.hasActual_ = true;
      entry.actualIdx_ = actualIdx;
      entry.actualType_ = actual.type();
      actualIdxToFormalIdx_[actualIdx] = entryIdx;
    }
    actualIdx++;
  }

  if (!untyped->isTypeConstructor()) {
    missingIteratorActuals_ = untyped->isIterator();
    bool validMapping = true;

    // Make sure that any remaining formals are matched by name
    // or have a default value.
    // This is left out for type constructors because presently
    // a partial instantiation is provided by simply leaving out arguments.
    for (;entryIdx < numEntries; entryIdx++) {
      FormalActual& entry = byFormalIdx_[entryIdx];
      if (entry.actualIdx_ < 0) {
        if (entry.hasDefault() == false) {
          // formal was not provided and there is no default value
          validMapping = false;
          failingFormalIdx_ = entry.formalIdx();

          // The mapping is invalid, but if it's an iterator, we should
          // continue and check if it's only invalid because of
          // iterKind/followThis. If it's not an iterator, no seeking
          // is necessary.
          if (!untyped->isIterator()) {
            break;
          }

          if (untyped->formalName(entryIdx) != USTR("tag") &&
              untyped->formalName(entryIdx) != USTR("followThis")) {
            missingIteratorActuals_ = false;
            break;
          }
        }
      }
    }

    return validMapping;
  }

  return true;
}

static bool
syntacticallyGenericFieldsPriorToIdHaveSubs(Context* context,
                                           const CompositeType* ct,
                                           ID fieldId) {
  if (auto bct = ct->toBasicClassType()) {
    if (auto parentCt = bct->parentClassType()) {
      if (!syntacticallyGenericFieldsPriorToIdHaveSubs(context, parentCt, fieldId)) {
        return false;
      }
    }
  }

  // Compute the fields without types so that we can iterate the fields.
  ResolutionContext rc(context);
  auto& fieldsForOrder = fieldsForTypeDecl(&rc, ct, DefaultsPolicy::IGNORE_DEFAULTS,
                                           /* syntaxOnly */ true);
  for (int i = 0; i < fieldsForOrder.numFields(); i++) {
    auto ithField = fieldsForOrder.fieldDeclId(i);
    if (ithField == fieldId) {
      // We haven't found a generic field without a sub, so we can stop.
      return true;
    }

    // Skip over concrete types, since they don't need substitutions.
    if (!isFieldSyntacticallyGeneric(context, ithField, nullptr))
      continue;

    // Found a generic type without a substitution.
    if (ct->substitutions().find(ithField) == ct->substitutions().end()) {
      return false;
    }
  }

  return true;
}


void ResolvedFields::validateFieldGenericity(Context* context, const types::CompositeType* fieldsOfType) const {
  // Check if all fields preceding the current field have substitutions.
  // We do this in case this ResolvedFields is computed for a particular
  // (multi)decl, and thus its first field is not actually the first in the
  // fieldsOfType.
  //
  // We want to skip checking for genericity if some preceding generic fields
  // haven't been instantiated, since this means the type may be a partial
  // instantiation.

  if (fields_.empty()) {
    return;
  }

  if (!syntacticallyGenericFieldsPriorToIdHaveSubs(context, fieldsOfType,
                                                  fields_[0].declId)) {
    return;
  }

  for (auto& field : fields_) {
    auto isGeneric = isFieldSyntacticallyGeneric(context, field.declId, nullptr);
    if (!isGeneric) {
      // Check if the type is actually concrete according to the type we have stored.
      std::set<const Type*> ignore = {fieldsOfType};
      auto g = getTypeGenericityIgnoring(context, field.type, ignore);
      if (g != Type::CONCRETE) {
        auto ast = parsing::idToAst(context, field.declId)->toDecl();
        CHPL_REPORT(context, SyntacticGenericityMismatch, ast, g, Type::CONCRETE, field.type);
      }
    } else {
      // Type is syntactically generic. If it doesn't have a substitution,
      // the type may be partially instantiated, so the remaining fields,
      // which may eventually be concrete, might not be at this time, and that's
      // not an error.
      if (fieldsOfType->substitutions().find(field.declId) == fieldsOfType->substitutions().end()) {
        break;
      }
    }
  }
}

void ResolvedFields::finalizeFields(Context* context, bool syntaxOnly) {
  bool anyGeneric = false ;
  bool allGenHaveDefault = true; // all generic fields have default init
                                 // -- vacuously true if there are no generic

  std::set<const Type*> ignore;
  ignore.insert(type_);

  // look at the fields and compute the summary information
  for (const auto& field : fields_) {
    Type::Genericity g;

    if (syntaxOnly) {
      if (!isFieldSyntacticallyGenericIgnoring(context, field.declId, nullptr, ignore)) {
        g = Type::CONCRETE;
      } else {
        // In syntaxOnly mode, the field type is only set from substitutions;
        // if there are none, it's left unknown, which, if the field is
        // syntactically generic, means it's generic.
        if (field.type.isUnknownKindOrType()) {
          g = Type::GENERIC;
        } else {
          g = getTypeGenericityIgnoring(context, field.type, ignore);
        }
      }
    } else {
      g = getTypeGenericityIgnoring(context, field.type, ignore);
    }

    if (g != Type::CONCRETE) {
      if (!field.hasDefaultValue) {
        allGenHaveDefault = false;
      }
      anyGeneric = true;
    }
  }

  isGeneric_ = anyGeneric;
  allGenericFieldsHaveDefaultValues_ = allGenHaveDefault;
}

const owned<TypedFnSignature>&
TypedFnSignature::getTypedFnSignature(Context* context,
                    const UntypedFnSignature* untypedSignature,
                    std::vector<types::QualifiedType> formalTypes,
                    TypedFnSignature::WhereClauseResult whereClauseResult,
                    bool needsInstantiation,
                    bool isRefinementOnly,
                    const TypedFnSignature* instantiatedFrom,
                    const TypedFnSignature* parentFn,
                    Bitmap formalsInstantiated,
                    OuterVariables outerVariables) {
  QUERY_BEGIN(getTypedFnSignature, context,
              untypedSignature, formalTypes, whereClauseResult,
              needsInstantiation, isRefinementOnly, instantiatedFrom, parentFn,
              formalsInstantiated,
              outerVariables);

  auto result = toOwned(new TypedFnSignature(untypedSignature,
                                             std::move(formalTypes),
                                             whereClauseResult,
                                             needsInstantiation,
                                             isRefinementOnly,
                                             instantiatedFrom,
                                             parentFn,
                                             std::move(formalsInstantiated),
                                             std::move(outerVariables)));

  return QUERY_END(result);
}

const TypedFnSignature*
TypedFnSignature::get(Context* context,
                      const UntypedFnSignature* untypedSignature,
                      std::vector<types::QualifiedType> formalTypes,
                      TypedFnSignature::WhereClauseResult whereClauseResult,
                      bool needsInstantiation,
                      const TypedFnSignature* instantiatedFrom,
                      const TypedFnSignature* parentFn,
                      Bitmap formalsInstantiated,
                      OuterVariables outerVariables) {
  return getTypedFnSignature(context, untypedSignature,
                             std::move(formalTypes),
                             whereClauseResult,
                             needsInstantiation,
                             /* isRefinementOnly */ false,
                             instantiatedFrom,
                             parentFn,
                             std::move(formalsInstantiated),
                             std::move(outerVariables)).get();
}

const TypedFnSignature*
TypedFnSignature::getInferred(
                      Context* context,
                      std::vector<types::QualifiedType> formalTypes,
                      const TypedFnSignature* inferredFrom) {
  return getTypedFnSignature(context,
                             inferredFrom->untyped(),
                             formalTypes,
                             inferredFrom->whereClauseResult(),
                             inferredFrom->needsInstantiation(),
                             /* isRefinementOnly */ true,
                             inferredFrom->inferredFrom(),
                             inferredFrom->parentFn(),
                             inferredFrom->formalsInstantiatedBitmap(),
                             inferredFrom->outerVariables()).get();
}

const TypedFnSignature*
TypedFnSignature::substitute(Context* context,
                             const PlaceholderMap& subs) const {
  std::vector<QualifiedType> newFormalTypes;
  for (const auto& formalType : formalTypes_) {
    newFormalTypes.push_back(formalType.substitute(context, subs));
  }

  // TODO: do we need to substitute in outer variables' stored types?

  return getTypedFnSignature(context, untyped(),
                             std::move(newFormalTypes),
                             whereClauseResult(),
                             needsInstantiation(),
                             isRefinementOnly_,
                             instantiatedFrom(),
                             parentFn(),
                             formalsInstantiatedBitmap(),
                             outerVariables()).get();
}

void TypedFnSignature::stringify(std::ostream& ss,
                                 chpl::StringifyKind stringKind) const {


  if (!id().isEmpty()) {
    ss << "id ";
    id().stringify(ss, stringKind);
    ss << " ";
  }
  untyped()->name().stringify(ss, stringKind);
  ss << "(";
  int nFormals = numFormals();
  for (int i = 0; i < nFormals; i++) {
    if (i != 0) {
      ss << ", ";
    }
    formalName(i).stringify(ss, stringKind);
    ss << " : ";
    formalType(i).stringify(ss, stringKind);
  }
  ss << ")";
}

bool TypedFnSignature::
fetchIterKindStr(Context* context, UniqueString& outIterKindStr) const {
  if (!isIterator()) return false;

  // Has to just be a serial iterator.
  if (numFormals() == 0 || (isMethod() && numFormals() == 1)) return true;

  auto ik = types::EnumType::getIterKindType(context);
  auto m = types::EnumType::getParamConstantsMapOrNull(context, ik);
  if (m == nullptr) return false;

  QualifiedType tagFormalType;
  bool foundTagFormal = false;
  UniqueString iterKindStr;

  // Loop over the formals since they could be in any position.
  for (int i = 0; i < numFormals(); i++) {
    if (formalName(i) == USTR("tag")) {
      foundTagFormal = true;
      tagFormalType = formalType(i);
      if (m != nullptr) {
        for (auto& p : *m) {
          if (formalType(i) != p.second) continue;
          iterKindStr = p.first;
          break;
        }
      }
    }
    if (foundTagFormal) break;
  }

  bool tagFormalMatches = tagFormalType.type() == ik &&
                          tagFormalType.param();
  if (tagFormalMatches) {
    CHPL_ASSERT(!iterKindStr.isEmpty());
    outIterKindStr = iterKindStr;
  }

  bool ret = !foundTagFormal || tagFormalMatches;
  return ret;
}

void CandidatesAndForwardingInfo::stringify(
    std::ostream& ss, chpl::StringifyKind stringKind) const {
  ss << "CandidatesAndForwardingInfo: ";
  ss << "(candidates) ";
  for (const auto& candidate : candidates) {
    candidate->stringify(ss, stringKind);
  }
  ss << "(forwarding info) ";
  for (const auto& info : forwardingInfo) {
    info.stringify(ss, stringKind);
  }
}


// Note (Daniel): the code for 'overloaded' below comes from cppreference:
//   https://en.cppreference.com/w/cpp/utility/variant/visit2
//
template<class... Ts>
struct overloaded : Ts... { using Ts::operator()...; };
// explicit deduction guide (not needed as of C++20)
template<class... Ts>
overloaded(Ts...) -> overloaded<Ts...>;

void ApplicabilityResult::mark(Context* context) const {
  std::visit(overloaded {
    [context](const ID& id) { id.mark(context); },
    [context](const TypedFnSignature* fn) { context->markPointer(fn); },
    [context](const UntypedFnSignature* ufs) { context->markPointer(ufs); }
  }, rejected_);
  context->markPointer(candidate_);
  (void) candidateReason_; // nothing to mark
  (void) formalReason_; // nothing to mark
  (void) formalIdx_; // nothing to mark
}

const ID& ApplicabilityResult::idForErr() const {
  return std::visit(overloaded {
    [](const ID& id) -> const ID& { return id; },
    [](const UntypedFnSignature* ufs) -> const ID& { return ufs->id(); },
    [](const TypedFnSignature* fn) -> const ID& { return fn->id(); }
  }, rejected_);
}

const UntypedFnSignature* ApplicabilityResult::untypedForErr() const {
  return std::visit(overloaded {
    [](const ID& id) -> const UntypedFnSignature* { return nullptr; },
    [](const UntypedFnSignature* ufs) { return ufs; },
    [](const TypedFnSignature* fn) { return fn->untyped(); }
  }, rejected_);
}

const TypedFnSignature* ApplicabilityResult::initialForErr() const {
  return std::visit(overloaded {
    [](const ID& id) -> const TypedFnSignature* { return nullptr; },
    [](const UntypedFnSignature* ufs) -> const TypedFnSignature* { return nullptr; },
    [](const TypedFnSignature* fn) { return fn; }
  }, rejected_);
}

void CallInfoActual::stringify(std::ostream& ss,
                               chpl::StringifyKind stringKind) const {
  if (!byName_.isEmpty()) {
    byName_.stringify(ss, stringKind);
    ss << "=";
  }

  type_.stringify(ss, stringKind);
}

void CallInfo::stringify(std::ostream& ss,
                         chpl::StringifyKind stringKind) const {
  if (stringKind != StringifyKind::CHPL_SYNTAX) {
    ss << "CallInfo with name '";
    name_.stringify(ss, stringKind);
    ss << "'";
  } else {
    name_.stringify(ss, stringKind);
  }
  if (calledType_ != QualifiedType()) {
    ss << " receiver type: ";
    calledType_.stringify(ss, stringKind);
  }
  if (stringKind != StringifyKind::CHPL_SYNTAX) {
    ss << " isMethodCall=" << isMethodCall_;
    ss << " isOpCall=" << isOpCall_;
    ss << " hasQuestionArg=" << hasQuestionArg_;
    ss << " ";
  }
  ss << "(";
  bool first = true;
  for (const auto& actual: actuals()) {
    if (first) {
      first = false;
    } else {
      ss << ",";
    }
    actual.stringify(ss, stringKind);
  }
  ss << ")";
}

void PoiInfo::accumulate(const PoiInfo& addPoiInfo) {
  poiFnIdsUsed_.insert(addPoiInfo.poiFnIdsUsed_.begin(),
                       addPoiInfo.poiFnIdsUsed_.end());
  recursiveFnsUsed_.insert(addPoiInfo.recursiveFnsUsed_.begin(),
                           addPoiInfo.recursiveFnsUsed_.end());
}

void PoiInfo::accumulateRecursive(const TypedFnSignature* signature,
                                  const PoiScope* poiScope) {
  recursiveFnsUsed_.insert(std::make_pair(signature, poiScope));
}

// this is a resolved function
// check is a not-yet-resolved function
bool PoiInfo::canReuse(const PoiInfo& check) const {
  CHPL_ASSERT(resolved_ && !check.resolved_);

  if (poiScope_ == check.poiScope_) {
    // if the POI scopes are the same, then we can reuse
    return true;
  }

  if (poiFnIdsUsed_.empty() && recursiveFnsUsed_.empty()) {
    // if we have no POI functions used, then we can reuse
    return true;
  }

  // Performance TODO: consider function names etc -- see PR #16261
  return false;
}

size_t hashPromotedFormalMap(const PromotedFormalMap& map) {
  return hashUnorderedMap(map);
}

MostSpecificCandidate
MostSpecificCandidate::fromTypedFnSignature(ResolutionContext* rc,
                                            const TypedFnSignature* fn,
                                            const FormalActualMap& faMap,
                                            const Scope* scope,
                                            const PoiScope* poiScope,
                                            const PromotedFormalMap& promotedFormals) {
  auto newFaMap = faMap;

  // Earlier, we didn't resolve the body of the function, but when it's an
  // initializer, it can have substitution-producing statements such
  // as `this.typeField = int`. Now that we have picked this candidate
  // as most specific, it's safe to resolve the body without worrying about
  // spurious errors from other andidates.
  if (fn->isInitializer()) {
    auto instantiationPoiScope =
      Resolver::poiScopeOrNull(rc->context(), fn, scope, poiScope);
    auto rf = resolveFunction(rc, fn, instantiationPoiScope, /* skipIfRunning */ true);

    if (rf) {
      fn = rf->signature();
      newFaMap.updateReceiverType(fn);
    }
  }

  int coercionFormal = -1;
  int coercionActual = -1;
  for (auto fa : faMap.byFormals()) {
    auto& formalType = fa.formalType();
    auto& actualType = fa.actualType();

    if (!formalType.type() || !actualType.type()) continue;

    auto canPassFn =
      actualType.kind() == QualifiedType::INIT_RECEIVER ? canPassScalar
                                                        : canPass;
    // An exception is made for the case where we have to convert a tuple to
    // its referential equivalent, which doesn't count as coercion in the
    // conventional Chapel sense.
    auto got = canPassFn(rc->context(), actualType, formalType);
    if (got.converts() && formalType.kind() == QualifiedType::CONST_REF &&
        got.conversionKind() != CanPassResult::TO_REFERENTIAL_TUPLE) {
      coercionFormal = fa.formalIdx();
      coercionActual = fa.actualIdx();
      break;
    }
  }

  return MostSpecificCandidate(fn, std::move(newFaMap), promotedFormals, coercionFormal, coercionActual);
}

MostSpecificCandidate
MostSpecificCandidate::fromTypedFnSignature(ResolutionContext* rc,
                                            const TypedFnSignature* fn,
                                            const CallInfo& ci,
                                            const Scope* scope,
                                            const PoiScope* poiScope,
                                            const PromotedFormalMap& promotedFormals) {
  auto faMap = FormalActualMap(fn, ci);
  return MostSpecificCandidate::fromTypedFnSignature(rc, fn, faMap, scope, poiScope, promotedFormals);
}

void MostSpecificCandidate::stringify(std::ostream& ss,
                          chpl::StringifyKind stringKind) const {
  if (fn_) fn_->stringify(ss, stringKind);
}

void
MostSpecificCandidates::inferOutFormals(ResolutionContext* rc,
                                        const PoiScope* instantiationPoiScope) {
  for (int i = 0; i < NUM_INTENTS; i++) {
    if (MostSpecificCandidate& c = candidates[i]) {
      constexpr auto f = chpl::resolution::inferOutFormals;
      c.fn_ = f(rc, c.fn(), instantiationPoiScope);
    }
  }
}

void MostSpecificCandidates::stringify(std::ostream& ss,
                                       chpl::StringifyKind stringKind) const {
  auto onlyFn = only();
  if (onlyFn) {
    ss << " calls ";
    onlyFn.stringify(ss, stringKind);
  } else {
    if (auto& c = bestRef()) {
      ss << " calls ref ";
      c.stringify(ss, stringKind);
    }
    if (auto& c = bestConstRef()) {
      ss << " calls const ref ";
      c.stringify(ss, stringKind);
    }
    if (auto& c = bestValue()) {
      ss << " calls value ";
      c.stringify(ss, stringKind);
    }
  }
}

void CallResolutionResult::stringify(std::ostream& ss,
                                     chpl::StringifyKind stringKind) const {
  mostSpecific_.stringify(ss, stringKind);
  ss << " : ";
  exprType_.stringify(ss, stringKind);
}

CallScopeInfo CallScopeInfo::forNormalCall(const Scope* scope, const PoiScope* poiScope) {
  return CallScopeInfo(scope, scope, poiScope);
}

CallScopeInfo CallScopeInfo::forQualifiedCall(Context* context, const ID& moduleId,
                                      const Scope* scope, const PoiScope* poiScope) {
  auto moduleScope = scopeForModule(context, moduleId);
  return CallScopeInfo(scope, moduleScope, poiScope);
}

CallScopeInfo CallScopeInfo::forIteratorOverloadSearch(const Scope* callScope,
                                                       const Scope* iteratorDefinitionScope,
                                                       const PoiScope* poiScope) {
  return CallScopeInfo(callScope, iteratorDefinitionScope, poiScope);
}

const char* AssociatedAction::kindToString(Action a) {
  switch (a) {
    case ASSIGN:
      return "assign";
    case COPY_INIT:
      return "copy-init";
    case DEFAULT_INIT:
      return "default-init";
    case CUSTOM_COPY_INIT:
      return "custom-copy-init";
    case INIT_OTHER:
      return "init-from-other";
    case DEINIT:
      return "deinit";
    case ITERATE:
      return "these";
    case NEW_INIT:
      return "new-init";
    case REDUCE_SCAN:
      return "reduce-scan";
    case REDUCE_ASSIGN:
      return "reduce-assign";
    case INFER_TYPE:
      return "infer-type";
    case COMPARE:
      return "compare";
    case RUNTIME_TYPE:
      return "runtime-type";
    case ENTER_CONTEXT:
      return "enter-context";
    case EXIT_CONTEXT:
      return "exit-context";
    // no default to get a warning if new Actions are added
  }

  return "<unknown>";
}

void AssociatedAction::stringify(std::ostream& ss,
                                 chpl::StringifyKind stringKind) const {
  const char* kind = AssociatedAction::kindToString(action_);

  ss << "assoc " << kind;
  if (fn_ != nullptr) {
    ss << " fn=";
    fn_->stringify(ss, stringKind);
  }
  if (!id_.isEmpty()) {
    ss << " id=";
    id_.stringify(ss, stringKind);
  }
  if (!type_.isUnknown()) {
    ss << " type=";
    type_.stringify(ss, stringKind);
  }
}

void ResolvedExpression::stringify(std::ostream& ss,
                                   chpl::StringifyKind stringKind) const {
  ss << " : ";
  type_.stringify(ss, stringKind);
  ss << " ; ";
  if (!toId_.isEmpty()) {
    ss << " refers to ";
    toId_.stringify(ss, stringKind);
  } else {
    mostSpecific_.stringify(ss, stringKind);
  }

  for (auto a : associatedActions_) {
    a.stringify(ss, stringKind);
  }
}

void
ResolutionResultByPostorderID::stringify(std::ostream& ss,
                                         chpl::StringifyKind stringKind) const {
  std::vector<int> keys;
  for (const auto& pair : map) {
    keys.push_back(pair.first);
  }

  std::sort(keys.begin(), keys.end());

  size_t maxIdWidth = 0;
  for (auto key : keys) {
    auto id = ID(symbolId_.symbolPath(), key, -1);
    if (id.str().size() > maxIdWidth)
      maxIdWidth = id.str().size();
  }

  for (auto key : keys) {
    auto id = ID(symbolId_.symbolPath(), key, -1);

    // output the ID
    std::cout << std::setw(maxIdWidth) << std::left << id.str();
    // restore format to default
    std::cout.copyfmt(std::ios(NULL));

    if (const ResolvedExpression* re = byIdOrNull(id)) {
      re->stringify(std::cout, chpl::StringifyKind::CHPL_SYNTAX);
    }
    std::cout << "\n";
  }
}

void
ResolvedFunction::stringify(std::ostream& ss,
                                         chpl::StringifyKind stringKind) const {
  ss << "ResolvedFunction: " << std::endl;

  if (signature_) {
    signature_->stringify(ss, stringKind);
  } else {
    ss << "<no signature>";
  }
  ss << std::endl;

  ss << "return intent: ";
  ss << Function::returnIntentToString(returnIntent_);
  ss << std::endl;

  ss << "return type: ";
  returnType_.stringify(ss, chpl::StringifyKind::CHPL_SYNTAX);
  ss << std::endl;

  ss << "----- resolution results -----" << std::endl;
  resolutionById_.stringify(ss, chpl::StringifyKind::CHPL_SYNTAX);
  ss << "----- end resolution results -----" << std::endl;
}


/**
  Find method receiver aggregate decl ID (for use when scope resolving).
  This function does not support certain patterns; it should be viewed
  as an approximation that should be replaced by full resolution.
  */
static const ID& methodReceiverTypeIdForMethodId(Context* context,
                                                 ID methodId) {
  QUERY_BEGIN(methodReceiverTypeIdForMethodId, context, methodId);

  ID result;

  if (auto ast = parsing::idToAst(context, methodId)) {
    if (auto fn = ast->toFunction()) {
      if (fn->isMethod()) {
        if (fn->isPrimaryMethod()) {
          // Find the containing aggregate ID
          result = parsing::idToParentId(context, methodId);
        } else {
          // Resolve the method receiver to an ID
          ResolutionResultByPostorderID r;
          auto visitor =
            Resolver::createForScopeResolvingFunction(context, fn, r);

          const AstNode* typeExpr = nullptr;
          if (auto thisFormal = fn->thisFormal()) {
            typeExpr = thisFormal->typeExpression();
          }

          if (typeExpr) {
            fn->thisFormal()->traverse(visitor);
            ResolvedExpression& re = r.byAst(typeExpr);
            result = re.toId();
          }

          // result might not have been set but that is OK;
          // we want to ignore errors here while just scope resolving.
        }
      }
    } else if (ast->isForwardingDecl()) {
      // Find the containing aggregate ID
      result = parsing::idToParentId(context, methodId);
    }
  }

  return QUERY_END(result);
}
/**
  Find scopes for superclasses of a class.  The passed ID should refer to a
  Class declaration node.  If not, this function will return an empty vector.

  This function is temporary and should only be used in scopeResolveOnly mode.
  */
static const std::vector<const Scope*>&
gatherParentClassScopesForScopeResolving(Context* context, ID classDeclId) {
  QUERY_BEGIN(gatherParentClassScopesForScopeResolving, context, classDeclId);

  std::vector<const Scope*> result;

  bool encounteredError = false;
  auto ast = parsing::idToAst(context, classDeclId);
  if (!ast) return QUERY_END(result);

  auto c = ast->toClass();
  if (!c || c->numInheritExprs() == 0) return QUERY_END(result);

  const uast::AstNode* lastParentClass = nullptr;
  ID parentClassDeclId;
  for (auto inheritExpr : c->inheritExprs()) {
    // Resolve the parent class type expression
    ResolutionResultByPostorderID r;
    auto visitor =
      Resolver::createForParentClassScopeResolve(context, c, inheritExpr, r);
    // Parsing excludes non-identifiers as parent class expressions.
    //
    // Intended to avoid calling methodReceiverScopes() recursively.
    // Uses the empty 'savecReceiverScopes' because the class expression
    // can't be a method anyways.
    bool ignoredMarkedGeneric = false;
    auto inherit = Class::getUnwrappedInheritExpr(inheritExpr,
                                                ignoredMarkedGeneric);
    inherit->traverse(visitor);


    ResolvedExpression& re = r.byAst(inherit);
    if (re.toId().isEmpty()) {
      context->error(inheritExpr, "invalid parent class expression");
      encounteredError = true;
      break;
    } else if (parsing::idToTag(context, re.toId()) == uast::asttags::Interface)
    {
      // this is an interface; ignore it for the purposes of parent scopes.
    } else {
      if (lastParentClass) {
        reportInvalidMultipleInheritance(context, c, lastParentClass, inheritExpr);
        encounteredError = true;
        break;
      }
      lastParentClass = inheritExpr;

      result.push_back(scopeForId(context, re.toId()));
      parentClassDeclId = re.toId();
      // keep going through the list of parent expressions. hitting
      // another parent expression that's a class after this point
      // will result in an error. When we're done with other parent
      // expressions, the loop will continue to searching for the
      // parent classes of this parent class.
    }
  }

  if (!encounteredError && !parentClassDeclId.isEmpty()) {
    const auto& parentScopes =
      gatherParentClassScopesForScopeResolving(context, parentClassDeclId);
    result.insert(result.end(), parentScopes.begin(), parentScopes.end());
  }

  return QUERY_END(result);
}

/* Gather scopes for a given receiver decl and all its parents
   (without using Types; for use when scope resolving) */
static SimpleMethodLookupHelper::ReceiverScopesVec
gatherReceiverAndParentScopesForDeclId(Context* context, ID aggregateDeclId) {
  SimpleMethodLookupHelper::ReceiverScopesVec scopes;

  if (aggregateDeclId.isEmpty()) {
    return scopes;
  }

  // use temporary code to scope resolve the parent class Identifiers
  scopes.push_back(scopeForId(context, aggregateDeclId));
  // if it's a class type, also gather the parent class scopes
  auto tag = parsing::idToTag(context, aggregateDeclId);
  if (asttags::isClass(tag)) {
    const std::vector<const Scope*>& v =
      gatherParentClassScopesForScopeResolving(context, aggregateDeclId);

    scopes.append(v.begin(), v.end());
  }

  return scopes;
}

static const SimpleMethodLookupHelper&
simpleMethodLookupQuery(Context* context, ID typeId) {
  QUERY_BEGIN(simpleMethodLookupQuery, context, typeId);

  auto vec = gatherReceiverAndParentScopesForDeclId(context, typeId);
  auto result = SimpleMethodLookupHelper(typeId, std::move(vec));

  return QUERY_END(result);
}

static const TypedMethodLookupHelper&
typedMethodLookupQuery(Context* context, QualifiedType receiverType, bool checkScalarTypes);

/* Gather scopes for a given receiver type and all its parents */
static TypedMethodLookupHelper::ReceiverScopesVec
gatherReceiverAndParentScopesForType(Context* context,
                                     const types::Type* thisType,
                                     bool checkScalarTypes) {
  TypedMethodLookupHelper::ReceiverScopesVec scopes;

  if (thisType != nullptr) {
    if (const CompositeType* ct = thisType->getCompositeType()) {
      // add the scope declaring the type
      scopes.push_back(scopeForId(context, ct->id()));

      if (auto bct = ct->toBasicClassType()) {
        // add the scope for the manager type
        if (auto classType = thisType->toClassType()) {
          if (auto managerType = classType->managerRecordType(context)) {
            scopes.push_back(scopeForId(context, managerType->id()));
          }
        }
        // also add scopes for all superclass types
        auto cur = bct->parentClassType();
        while (cur != nullptr) {
          scopes.push_back(scopeForId(context, cur->id()));
          cur = cur->parentClassType();
        }
      }
    } else if (auto cptr = thisType->toCPtrType()) {
      scopes.push_back(scopeForId(context, cptr->id(context)));
    } else if (const ExternType* et = thisType->toExternType()) {
      scopes.push_back(scopeForId(context, et->id()));
    }


    auto thisQt = QualifiedType(QualifiedType::VAR, thisType);
    if (checkScalarTypes) {
      if (auto pqt = getPromotionType(context, thisQt, true); !pqt.isUnknownOrErroneous()) {
        // also get scopes from the element type, so that (array of things).method() works.
        auto& eltHelper = typedMethodLookupQuery(context, pqt, checkScalarTypes);
        std::copy(eltHelper.receiverScopes().begin(),
                  eltHelper.receiverScopes().end(),
                  std::back_inserter(scopes));
      }
    }
  }

  return scopes;
}

static const TypedMethodLookupHelper&
typedMethodLookupQuery(Context* context, QualifiedType receiverType, bool checkScalarTypes) {
  QUERY_BEGIN(typedMethodLookupQuery, context, receiverType, checkScalarTypes);

  auto vec = gatherReceiverAndParentScopesForType(context, receiverType.type(), checkScalarTypes);
  auto result = TypedMethodLookupHelper(receiverType, std::move(vec));

  return QUERY_END(result);
}


llvm::ArrayRef<const Scope*>
SimpleMethodLookupHelper::receiverScopes() const {
  return scopes_;
}

bool SimpleMethodLookupHelper::isReceiverApplicable(Context* context,
                                                    const ID& methodId) const {
  const ID& methodReceiverId =
    methodReceiverTypeIdForMethodId(context, methodId);
  if (methodReceiverId.isEmpty() || receiverTypeId_.isEmpty()) {
    return false; // empty IDs here mean something wasn't handled
  }

  return methodReceiverId == receiverTypeId_;
}


bool SimpleMethodLookupHelper::shouldCheckForTertiaryMethods(Context* context, const VisibilitySymbols* toCheck) const {
  return false; // no type information, so no clever tertiary method searching
}

void SimpleMethodLookupHelper::stringify(std::ostream& ss,
                                         chpl::StringifyKind stringKind) const {
  ss << "SimpleMethodLookupHelper ";
  receiverTypeId_.stringify(ss, stringKind);
  ss << " [";
  for (auto p : scopes_) {
    ss << " ";
    p->stringify(ss, stringKind);
  }
  ss << "]";
}

const SimpleMethodLookupHelper*
ReceiverScopeSimpleHelper::methodLookupForTypeId(Context* context,
                                                 const ID& typeId) const {
  const SimpleMethodLookupHelper& got =
    simpleMethodLookupQuery(context, typeId);
  if (!got.isEmpty())
    return &got;

  return nullptr;
}

const SimpleMethodLookupHelper*
ReceiverScopeSimpleHelper::methodLookupForMethodId(Context* context,
                                                   const ID& methodId) const {
  const ID& typeId = methodReceiverTypeIdForMethodId(context, methodId);
  if (typeId.isEmpty()) {
    return nullptr;
  }

  const SimpleMethodLookupHelper& got =
    simpleMethodLookupQuery(context, typeId);
  if (!got.isEmpty())
    return &got;

  return nullptr;
}

llvm::ArrayRef<const Scope*>
TypedMethodLookupHelper::receiverScopes() const {
  return scopes_;
}

bool TypedMethodLookupHelper::isReceiverApplicable(Context* context,
                                                   const ID& methodId) const {
  ResolutionContext rcval(context);
  const TypedFnSignature* tfs = typedSignatureInitialForId(&rcval, methodId);

  if (tfs && tfs->isMethod()) {
    QualifiedType methodRcvType = tfs->formalType(0);
    auto p = canPassScalar(context,
                           /* actual */ receiverType_, /* formal */ methodRcvType);
    return p.passes();
  }

  return false;
}

static std::vector<QualifiedType> const& importedTypesInVisibilitySymbols(Context* context, const VisibilitySymbols* symbols) {
  QUERY_BEGIN(importedTypesInVisibilitySymbols, context, symbols);
  std::vector<QualifiedType> result;

  if (symbols->kind() == VisibilitySymbols::ONLY_CONTENTS &&
      symbols->scope()->tag() == uast::asttags::Module) {
    for (auto& rename : symbols->names()) {
      auto& nameRE = resolveNameInModule(context, symbols->scope()->id(), rename.first);

      if (!nameRE.type().isUnknownOrErroneous()) result.push_back(nameRE.type());
    }
  }

  return QUERY_END(result);
}

bool TypedMethodLookupHelper::shouldCheckForTertiaryMethods(Context* context, const VisibilitySymbols* toCheck) const {
  auto receiverTypeAsType = QualifiedType(QualifiedType::TYPE, receiverType_.type());
  for (auto& namedType : importedTypesInVisibilitySymbols(context, toCheck)) {
    auto p = canPassScalar(context, receiverTypeAsType, namedType);
    if (p.passes()) return true;
  }
  return false;
}

void TypedMethodLookupHelper::stringify(std::ostream& ss,
                                        chpl::StringifyKind stringKind) const {
  ss << "TypedMethodLookupHelper ";
  receiverType_.stringify(ss, stringKind);
  ss << " [";
  for (auto p : scopes_) {
    ss << " ";
    p->stringify(ss, stringKind);
  }
  ss << "]";
}


const TypedMethodLookupHelper*
ReceiverScopeTypedHelper::methodLookupForType(Context* context,
                                              QualifiedType type,
                                              bool checkScalarTypes) const {
  if (const Type* typePtr = type.type()) {
    if (typePtr->getCompositeType() ||
        typePtr->isPtrType() ||
        typePtr->isExternType()) {
      // OK, it's a type that we need to gather receiver scopes for

      // normalize the kind of the type: always use one of VAR PARAM TYPE
      QualifiedType::Kind kind = type.kind();
      if (kind != QualifiedType::PARAM && kind != QualifiedType::TYPE) {
        kind = QualifiedType::VAR;
      }
      const TypedMethodLookupHelper& got =
        typedMethodLookupQuery(context, QualifiedType(kind, typePtr), checkScalarTypes);

      if (!got.isEmpty())
        return &got;
    }
  }

  return nullptr;
}

const TypedMethodLookupHelper*
ReceiverScopeTypedHelper::methodLookupForMethodId(Context* context,
                                                  const ID& methodId) const {

  if (resolvingMethodId_ == methodId) {
    return methodLookupForType(context, resolvingMethodReceiverType_);
  } else {
    ResolutionContext rcval(context);
    const TypedFnSignature* tfs = typedSignatureInitialForId(&rcval, methodId);
    if (tfs && tfs->isMethod()) {
      QualifiedType rcvType = tfs->formalType(0);
      return methodLookupForType(context, rcvType);
    }
  }

  return nullptr;
}

IMPLEMENT_DUMP(PoiInfo);
IMPLEMENT_DUMP(UntypedFnSignature);
IMPLEMENT_DUMP(UntypedFnSignature::FormalDetail);
IMPLEMENT_DUMP(TypedFnSignature);
IMPLEMENT_DUMP(ResolvedExpression);
IMPLEMENT_DUMP(ResolutionResultByPostorderID);
IMPLEMENT_DUMP(CallInfoActual);
IMPLEMENT_DUMP(CallInfo);
IMPLEMENT_DUMP(MostSpecificCandidates);
IMPLEMENT_DUMP(CallResolutionResult);
IMPLEMENT_DUMP(SimpleMethodLookupHelper);
IMPLEMENT_DUMP(TypedMethodLookupHelper);
IMPLEMENT_DUMP(ResolvedFunction);

} // end namespace resolution
} // end namespace chpl
