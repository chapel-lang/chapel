/*
 * Copyright 2021-2023 Hewlett Packard Enterprise Development LP
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

#include "chpl/resolution/resolution-types.h"

#include "chpl/parsing/parsing-queries.h"
#include "chpl/framework/global-strings.h"
#include "chpl/framework/query-impl.h"
#include "chpl/framework/update-functions.h"
#include "chpl/resolution/resolution-queries.h"
#include "chpl/types/TupleType.h"
#include "chpl/uast/Builder.h"
#include "chpl/uast/FnCall.h"
#include "chpl/uast/OpCall.h"
#include "chpl/uast/Formal.h"
#include "chpl/uast/Identifier.h"
#include "chpl/uast/For.h"
#include "chpl/uast/VarArgFormal.h"

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
                                          const AstNode* whereClause) {
  QUERY_BEGIN(getUntypedFnSignature, context,
              id, name, isMethod, isTypeConstructor, isCompilerGenerated,
               throws, idTag, kind, formals, whereClause);

  owned<UntypedFnSignature> result =
    toOwned(new UntypedFnSignature(id, name,
                                   isMethod, isTypeConstructor,
                                   isCompilerGenerated, throws, idTag, kind,
                                   std::move(formals), whereClause));

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
                        const uast::AstNode* whereClause) {
  return getUntypedFnSignature(context, id, name,
                               isMethod, isTypeConstructor,
                               isCompilerGenerated, throws, idTag, kind,
                               std::move(formals), whereClause).get();
}

static const UntypedFnSignature*
getUntypedFnSignatureForFn(Context* context, const uast::Function* fn) {
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
      } else if (auto varargs = decl->toVarArgFormal()) {
        name = varargs->name();

        // This should not be possible. Currently varargs with a default value
        // will be considered a syntax error.
        hasDefault = false;
        CHPL_ASSERT(varargs->initExpression() == nullptr);
      }

      auto fd = UntypedFnSignature::FormalDetail(name, hasDefault,
                                                 decl, decl->isVarArgFormal());
      formals.push_back(fd);
    }

    // find the unique-ified untyped signature
    result = UntypedFnSignature::get(context, fn->id(), fn->name(),
                                     fn->isMethod(),
                                     /* isTypeConstructor */ false,
                                     /* isCompilerGenerated */ false,
                                     /* throws */ fn->throws(),
                                     /* idTag */ asttags::Function,
                                     fn->kind(),
                                     std::move(formals), fn->whereClause());
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

static const UntypedFnSignature* const&
getUntypedFnSignatureForIdQuery(Context* context, ID functionId) {
  QUERY_BEGIN(getUntypedFnSignatureForIdQuery, context, functionId);

  const UntypedFnSignature* result = nullptr;
  const AstNode* ast = parsing::idToAst(context, functionId);

  if (ast != nullptr && ast->isFunction()) {
    result = getUntypedFnSignatureForFn(context, ast->toFunction());
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

CallInfo CallInfo::createSimple(const uast::FnCall* call) {
  // Pieces of the CallInfo we need to prepare.
  UniqueString name;
  QualifiedType calledType;
  bool isMethodCall = false;
  bool hasQuestionArg = false;
  std::vector<CallInfoActual> actuals;

  // set the name (simple cases only)
  if (auto called = call->calledExpression()) {
    if (auto id = called->toIdentifier()) {
      name = id->name();
    }
  }

  CHPL_ASSERT(!name.isEmpty());

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

  auto ret = CallInfo(name, calledType, isMethodCall, hasQuestionArg,
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

  const FnCall* fnCall = call->toFnCall();

  // Prepare the actuals of the call.
  for (int i = 0; i < call->numActuals(); i++) {
    auto actual = call->actual(i);

    if (isQuestionMark(actual)) {
      if (questionArg == nullptr) {
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
      if (fnCall && fnCall->isNamedActual(i)) {
        byName = fnCall->actualName(i);
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
}

CallInfo CallInfo::create(Context* context,
                          const Call* call,
                          const ResolutionResultByPostorderID& byPostorder,
                          bool raiseErrors,
                          std::vector<const uast::AstNode*>* actualAsts) {

  // Pieces of the CallInfo we need to prepare.
  UniqueString name;
  QualifiedType calledType;
  bool isMethodCall = false;
  const AstNode* questionArg = nullptr;
  std::vector<CallInfoActual> actuals;

  if (actualAsts != nullptr) {
    actualAsts->clear();
  }

  // Get the name of the called expression.
  if (auto op = call->toOpCall()) {
    name = op->op();
  } else if (auto called = call->calledExpression()) {
    if (auto calledIdent = called->toIdentifier()) {
      name = calledIdent->name();
    } else if (auto calledDot = called->toDot()) {
      name = calledDot->field();
    } else {
      CHPL_ASSERT(false && "Unexpected called expression");
    }
  }

  // Check for method call, maybe construct a receiver.
  if (!call->isOpCall()) {
    if (auto called = call->calledExpression()) {
      if (auto calledDot = called->toDot()) {

        const AstNode* receiver = calledDot->receiver();
        const ResolvedExpression& reReceiver = byPostorder.byAst(receiver);
        const QualifiedType& qtReceiver = reReceiver.type();

        // Check to make sure the receiver is a value or type.
        if (qtReceiver.kind() != QualifiedType::UNKNOWN &&
            qtReceiver.kind() != QualifiedType::FUNCTION &&
            qtReceiver.kind() != QualifiedType::MODULE) {

          actuals.push_back(CallInfoActual(qtReceiver, USTR("this")));
          if (actualAsts != nullptr) {
            actualAsts->push_back(receiver);
          }
          calledType = qtReceiver;
          isMethodCall = true;
        }
      }
    }
  }

  // Get the type of the called expression.
  if (isMethodCall == false) {
    if (auto calledExpr = call->calledExpression()) {
      const ResolvedExpression& r = byPostorder.byAst(calledExpr);
      calledType = r.type();

      if (calledType.kind() != QualifiedType::UNKNOWN &&
          calledType.kind() != QualifiedType::TYPE &&
          calledType.kind() != QualifiedType::FUNCTION) {
        // If e.g. x is a value (and not a function)
        // then x(0) translates to x.this(0)
        name = USTR("this");
        // add the 'this' argument as well
        isMethodCall = true;
        actuals.push_back(CallInfoActual(calledType, USTR("this")));
        if (actualAsts != nullptr) {
          actualAsts->push_back(calledExpr);
        }
        // and reset calledType
        calledType = QualifiedType(QualifiedType::FUNCTION, nullptr);
      }
    }
  }

  // Prepare the remaining actuals.
  prepareActuals(context, call, byPostorder, raiseErrors,
                 actuals, questionArg, actualAsts);

  if (actualAsts != nullptr) {
    CHPL_ASSERT(actualAsts->size() == actuals.size());
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
  // append the other actuals
  newActuals.insert(newActuals.end(), ci.actuals_.begin(), ci.actuals_.end());

  auto name = rename.isEmpty() ? ci.name_ : rename;
  return CallInfo(name, receiverType,
                  /* isMethodCall */ true,
                  ci.hasQuestionArg_,
                  ci.isParenless_,
                  std::move(newActuals));
}

void ResolutionResultByPostorderID::setupForSymbol(const AstNode* ast) {
  CHPL_ASSERT(Builder::astTagIndicatesNewIdScope(ast->tag()));

  symbolId = ast->id();
}
void ResolutionResultByPostorderID::setupForSignature(const Function* func) {
  symbolId = func->id();
}
void ResolutionResultByPostorderID::setupForParamLoop(
    const For* loop, ResolutionResultByPostorderID& parent) {
  this->symbolId = parent.symbolId;
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
      entry.hasDefault_ = untyped->formalHasDefault(i);

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
      if (formalQT.type() != nullptr) {
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
        } else if (formalQT.type() != nullptr) {
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
        entry.hasDefault_ = untyped->formalHasDefault(i);
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
    // Make sure that any remaining formals are matched by name
    // or have a default value.
    // This is left out for type constructors because presently
    // a partial instantiation is provided by simply leaving out arguments.
    for (;entryIdx < numEntries; entryIdx++) {
      FormalActual& entry = byFormalIdx_[entryIdx];
      if (entry.actualIdx_ < 0) {
        if (entry.hasDefault() == false) {
          // formal was not provided and there is no default value
          failingFormalIdx_ = entry.formalIdx();
          return false;
        }
      }
    }
  }

  return true;
}

void ResolvedFields::finalizeFields(Context* context) {
  bool anyGeneric = false ;
  bool allGenHaveDefault = true; // all generic fields have default init
                                 // -- vacuously true if there are no generic

  std::set<const Type*> ignore;
  ignore.insert(type_);

  // look at the fields and compute the summary information
  for (const auto& field : fields_) {
    auto g = getTypeGenericityIgnoring(context, field.type, ignore);
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
                    Bitmap formalsInstantiated) {
  QUERY_BEGIN(getTypedFnSignature, context,
              untypedSignature, formalTypes, whereClauseResult,
              needsInstantiation, isRefinementOnly, instantiatedFrom, parentFn,
              formalsInstantiated);

  auto result = toOwned(new TypedFnSignature(untypedSignature,
                                             std::move(formalTypes),
                                             whereClauseResult,
                                             needsInstantiation,
                                             isRefinementOnly,
                                             instantiatedFrom,
                                             parentFn,
                                             std::move(formalsInstantiated)));

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
                      Bitmap formalsInstantiated) {
  return getTypedFnSignature(context, untypedSignature,
                             std::move(formalTypes),
                             whereClauseResult,
                             needsInstantiation,
                             /* isRefinementOnly */ false,
                             instantiatedFrom,
                             parentFn,
                             std::move(formalsInstantiated)).get();
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
                             inferredFrom->formalsInstantiatedBitmap()).get();
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

  return false; // TODO -- consider function names etc -- see PR #16261
}

void
MostSpecificCandidates::inferOutFormals(Context* context,
                                        const PoiScope* instantiationPoiScope) {
  for (int i = 0; i < NUM_INTENTS; i++) {
    const TypedFnSignature*& c = candidates[i];
    if (c != nullptr) {
      c = chpl::resolution::inferOutFormals(context, c, instantiationPoiScope);
    }
  }
}

void MostSpecificCandidates::stringify(std::ostream& ss,
                                       chpl::StringifyKind stringKind) const {
  auto onlyFn = only();
  if (onlyFn) {
    ss << " calls ";
    onlyFn->stringify(ss, stringKind);
  } else {
    if (auto sig = bestRef()) {
      ss << " calls ref ";
      sig->stringify(ss, stringKind);
    }
    if (auto sig = bestConstRef()) {
      ss << " calls const ref ";
      sig->stringify(ss, stringKind);
    }
    if (auto sig = bestValue()) {
      ss << " calls value ";
      sig->stringify(ss, stringKind);
    }
  }
}

void CallResolutionResult::stringify(std::ostream& ss,
                                     chpl::StringifyKind stringKind) const {
  mostSpecific_.stringify(ss, stringKind);
  ss << " : ";
  exprType_.stringify(ss, stringKind);
}


const char* AssociatedAction::kindToString(Action a) {
  switch (a) {
    case ASSIGN:
      return "assign";
    case COPY_INIT:
      return "copy-init";
    case DEFAULT_INIT:
      return "default-init";
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

IMPLEMENT_DUMP(PoiInfo);
IMPLEMENT_DUMP(UntypedFnSignature);
IMPLEMENT_DUMP(UntypedFnSignature::FormalDetail);
IMPLEMENT_DUMP(TypedFnSignature);
IMPLEMENT_DUMP(ResolvedExpression);
IMPLEMENT_DUMP(CallInfoActual);
IMPLEMENT_DUMP(CallInfo);
IMPLEMENT_DUMP(MostSpecificCandidates);
IMPLEMENT_DUMP(CallResolutionResult);

} // end namespace resolution
} // end namespace chpl
