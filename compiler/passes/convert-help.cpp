/*
 * Copyright 2020-2024 Hewlett Packard Enterprise Development LP
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

/* This file implements functionality that is shared
   by convert-uast.cpp and convert-typed-uast.cpp.
 */

#include "convert-help.h"

#include "chpl/parsing/parsing-queries.h"
#include "chpl/uast/chpl-syntax-printer.h"

#include "stmt.h"

LLVMMetadataPtr
LoopAttributeInfo::tupleToLLVMMetadata(Context* context,
                                       const uast::Tuple* node) const {
  if (node->numActuals() != 1 && node->numActuals() != 2) return nullptr;

  if (!node->actual(0)->isStringLiteral()) return nullptr;
  auto attrName = node->actual(0)->toStringLiteral()->value().astr(context);

  if (node->numActuals() == 1) {
    return LLVMMetadata::construct(attrName);
  } else {
    auto attrVal = node->actual(1);

    if (auto str = attrVal->toStringLiteral())
      return LLVMMetadata::constructString(attrName, str->value().astr(context));
    else if (auto int_ = attrVal->toIntLiteral())
      return LLVMMetadata::constructInt(attrName, int_->value());
    else if (auto bool_ = attrVal->toBoolLiteral())
      return LLVMMetadata::constructBool(attrName, bool_->value());
    else if (auto tup = attrVal->toTuple()) {
      auto v = tupleToLLVMMetadata(context, tup);
      if (v == nullptr) return nullptr;
      return LLVMMetadata::constructMetadata(attrName, v);
    }
    else return nullptr;
  }
}

LLVMMetadataPtr
LoopAttributeInfo::nodeToLLVMMetadata(Context* context,
                                      const uast::AstNode* node) const {
  if (node->isTuple()) {
    return tupleToLLVMMetadata(context, node->toTuple());
  } else if (node->isStringLiteral()) {
    auto attrName = node->toStringLiteral()->value().astr(context);
    return LLVMMetadata::construct(attrName);
  } else {
    return nullptr;
  }
}

LLVMMetadataList
LoopAttributeInfo::buildLLVMMetadataList(Context* context,
                                         const uast::Attribute* node) const {
  LLVMMetadataList llvmAttrs;

  for (auto act: node->actuals()) {
    auto attr = nodeToLLVMMetadata(context, act);
    if (attr != nullptr) {
      llvmAttrs.push_back(attr);
    } else {
      auto loc = chpl::parsing::locateId(context, node->id());
      std::string msg = "Invalid value for '" + node->name().str() + "'";
      auto err = GeneralError::get(ErrorBase::ERROR, loc, msg);
      context->report(std::move(err));
    }
  }
  return llvmAttrs;
}

LLVMMetadataPtr
LoopAttributeInfo::buildAssertVectorize(const uast::Attribute* node) const {
  auto attrName = astr("chpl.loop.assertvectorized");
  return LLVMMetadata::constructBool(attrName, true);
}

void LoopAttributeInfo::readLlvmAttributes(Context* context,
                                           const uast::AttributeGroup* attrs) {
  if (auto a = attrs->getAttributeNamed(USTR("llvm.metadata"))) {
    auto userAttrs = buildLLVMMetadataList(context, a);
    this->llvmMetadata.insert(this->llvmMetadata.end(),
                              userAttrs.begin(),
                              userAttrs.end());
  }
  if (auto a = attrs->getAttributeNamed(USTR("llvm.assertVectorized"))) {
    this->llvmMetadata.push_back(buildAssertVectorize(a));
  }
}

void
LoopAttributeInfo::readNativeGpuAttributes(const uast::AttributeGroup* attrs) {
    this->assertOnGpuAttr = attrs->getAttributeNamed(USTR("assertOnGpu"));
    this->assertEligibleAttr = attrs->getAttributeNamed(USTR("gpu.assertEligible"));
    this->blockSizeAttr = attrs->getAttributeNamed(USTR("gpu.blockSize"));
    this->itersPerThreadAttr = attrs->getAttributeNamed(USTR("gpu.itersPerThread"));
}

bool LoopAttributeInfo::insertGpuEligibilityAssertion(BlockStmt* body) {
  bool inserted = false;
  if (assertOnGpuAttr) {
    body->insertAtTail(new CallExpr("chpl__assertOnGpuAttr"));
    inserted = true;
  }
  if (assertEligibleAttr) {
    body->insertAtTail(new CallExpr("chpl__gpuAssertEligibleAttr"));
    inserted = true;
  }
  return inserted;
}

static bool convertAttributeCall(UastConverter& converter,
                                 BlockStmt* body,
                                 const uast::Attribute* blockSizeAttr,
                                 const char* supportFn) {
  // In cases like compound promotion (A + 1 + 1), we might end up inserting
  // the GPU blockSize attribute several times, even though there's only
  // one place in the code where the attribute was created. To work around this,
  // add a unique identifier integer to each blockSize call. If blockSizes
  // are included twice, but they have a unique identifier that matches,
  // we can safely ignore the second one.
  static int counter = 0;

  if (blockSizeAttr) {
    auto newCall = new CallExpr(supportFn, new_IntSymbol(++counter));
    for (auto actual : blockSizeAttr->actuals()) {
      newCall->insertAtTail(converter.convertAST(actual));
    }
    body->insertAtTail(newCall);
    return true;
  }
  return false;
}

bool LoopAttributeInfo::insertBlockSizeCall(UastConverter& converter,
                                            BlockStmt* body) {
  return convertAttributeCall(converter, body,
                              blockSizeAttr, "chpl__gpuBlockSizeAttr");
}

bool LoopAttributeInfo::insertItersPerThreadCall(UastConverter& converter,
                                                 BlockStmt* body) {
  return convertAttributeCall(converter, body,
                           itersPerThreadAttr, "chpl__gpuItersPerThreadAttr");
}

BlockStmt* LoopAttributeInfo::createPrimitivesBlock(UastConverter& converter) {
  auto primBlock = new BlockStmt(BLOCK_SCOPELESS);
  primBlock->insertAtTail(new CallExpr(PRIM_GPU_PRIMITIVE_BLOCK));

  bool insertedAny = false;
  insertedAny |= insertGpuEligibilityAssertion(primBlock);
  insertedAny |= insertBlockSizeCall(converter, primBlock);
  insertedAny |= insertItersPerThreadCall(converter, primBlock);

  return insertedAny ? primBlock : nullptr;
}

void LoopAttributeInfo::insertPrimitivesBlockAtHead(UastConverter& converter,
                                                    BlockStmt* body) {
  if (auto primBlock = createPrimitivesBlock(converter)) {
    body->insertAtHead(primBlock);
  }
}


const char* convertLinkageNameAstr(const uast::Decl* node) {
  if (auto linkageName = node->linkageName()) {
    auto linkageStr = linkageName->toStringLiteral();
    INT_ASSERT(linkageStr);
    auto ret = astr(linkageStr->value());
    return ret;
  }

  return nullptr;
}

Flag convertFlagForDeclLinkage(const uast::AstNode* node) {
  if (auto decl = node->toDecl()) {
    switch (decl->linkage()) {
      case uast::Decl::EXTERN: return FLAG_EXTERN;
      case uast::Decl::EXPORT: return FLAG_EXPORT;
      default: return FLAG_UNKNOWN;
    }
  }

  return FLAG_UNKNOWN;
}


Flag convertPragmaToFlag(chpl::uast::PragmaTag pragma) {
  Flag ret = FLAG_UNKNOWN;
  switch (pragma) {
#define PRAGMA(name__, canParse__, parseStr__, desc__) \
    case chpl::uast::PRAGMA_ ## name__ : ret = FLAG_ ## name__; break;
#include "chpl/uast/PragmaList.h"
#undef PRAGMA
    default: break;
  }

  return ret;
}

void attachSymbolAttributes(Context* context,
                            const uast::Decl* node, Symbol* sym,
                            bool containedInLibraryFile) {
  if (containedInLibraryFile) {
    // If we are converting a symbol in a module from a .dyno
    // file, mark the symbol as precompiled.
    sym->addFlag(FLAG_PRECOMPILED);
  }

  const uast::AttributeGroup* attr;
  // use the query to get the AttributeGroup or you might miss the attributes
  // for children of multiDecl or tupleDecl
  attr = parsing::astToAttributeGroup(context, node);

  if (!attr) return;

  if (attr->isDeprecated()) {
    INT_ASSERT(!sym->hasFlag(FLAG_DEPRECATED));
    sym->addFlag(FLAG_DEPRECATED);

    auto msg = attr->deprecationMessage();
    if (!msg.isEmpty()) {
      sym->deprecationMsg = astr(msg);
    }
  } else {
    INT_ASSERT(attr->deprecationMessage().isEmpty());
  }

  if (attr->isUnstable()) {
    INT_ASSERT(!sym->hasFlag(FLAG_UNSTABLE));
    sym->addFlag(FLAG_UNSTABLE);

    auto msg = attr->unstableMessage();
    if (!msg.isEmpty()) {
      sym->unstableMsg = astr(msg);
    }
  } else {
    INT_ASSERT(attr->unstableMessage().isEmpty());
  }

  if (attr->isParenfulDeprecated()) {
    auto fnSym = toFnSymbol(sym);
    // post-parse checks have ensured that decl is a parenless function,
    // so the resulting symbol is a FnSymbol.
    INT_ASSERT(fnSym);

    INT_ASSERT(!sym->hasFlag(FLAG_DEPRECATED_PARENFUL));
    sym->addFlag(FLAG_DEPRECATED_PARENFUL);

    auto msg = attr->parenfulDeprecationMessage();
    if (!msg.isEmpty()) {
      fnSym->parenfulDeprecationMsg = astr(msg);
    }
  } else {
    INT_ASSERT(attr->parenfulDeprecationMessage().isEmpty());
  }

  for (auto pragma : attr->pragmas()) {
    Flag flag = convertPragmaToFlag(pragma);
    if (flag != FLAG_UNKNOWN) {
      sym->addFlag(flag);
    }
  }
}

void attachSymbolVisibility(const uast::Decl* node, Symbol* sym) {
  if (node->visibility() == uast::Decl::PRIVATE) {
    sym->addFlag(FLAG_PRIVATE);
  }
}

UnresolvedSymExpr* reservedWordToInternalName(UniqueString name) {
  static std::unordered_map<UniqueString, const char*> map = {
    { USTR("owned"), "_owned" },
    { USTR("shared"), "_shared" },
    { USTR("sync"), "_syncvar" },
    { USTR("domain"), "_domain" },
    { USTR("align"), "chpl_align" },
    { USTR("by"), "chpl_by" },

    // if "index" becomes an actual type, rather than magic:
    //
    // { USTR("index"), "_index" },
  };

  auto it = map.find(name);
  if (it != map.end()) {
    return new UnresolvedSymExpr(it->second);
  }

  return nullptr;
}

Expr* reservedWordRemapForIdent(UniqueString name) {
  if (name == USTR("?")) {
    return new SymExpr(gUninstantiated);
  } else if (name == USTR("unmanaged")) {
    return new SymExpr(dtUnmanaged->symbol);
  } else if (name == USTR("bytes")) {
    return new SymExpr(dtBytes->symbol);
  } else if (name == USTR("string")) {
    return new SymExpr(dtString->symbol);
  } else if (name == USTR("index")) {
    return new UnresolvedSymExpr("_index");
  } else if (name == USTR("nil")) {
    return new SymExpr(gNil);
  } else if (name == USTR("noinit")) {
    return new SymExpr(gNoInit);
  } else if (name == USTR("locale")) {
    return new SymExpr(dtLocale->symbol);
  } else if (name == USTR("uint")) {
    return new SymExpr(dtUInt[INT_SIZE_DEFAULT]->symbol);
  } else if (name == USTR("int")) {
    return new SymExpr(dtInt[INT_SIZE_DEFAULT]->symbol);
  } else if (name == USTR("real")) {
    return new SymExpr(dtReal[FLOAT_SIZE_DEFAULT]->symbol);
  } else if (name == USTR("complex")) {
    return new SymExpr(dtComplex[COMPLEX_SIZE_DEFAULT]->symbol);
  } else if (name == USTR("_")) {
    return new UnresolvedSymExpr("chpl__tuple_blank");
  } else if (name == USTR("void")) {
    return new SymExpr(dtVoid->symbol);
  }

  return reservedWordToInternalName(name);
}

LLVMMetadataList extractLlvmAttributesAndRejectOthers(Context* context, const uast::Loop* node) {
  auto loopAttributes = LoopAttributeInfo::fromExplicitLoop(context, node);
  if (loopAttributes.assertOnGpuAttr != nullptr) {
    CHPL_REPORT(context, InvalidGpuAttribute, node,
                loopAttributes.assertOnGpuAttr);
  } else if (loopAttributes.assertEligibleAttr != nullptr) {
    CHPL_REPORT(context, InvalidGpuAttribute, node,
                loopAttributes.assertEligibleAttr);
  }
  return std::move(loopAttributes.llvmMetadata);
}

RetTag convertRetTag(uast::Function::ReturnIntent returnIntent) {
  switch (returnIntent) {
    case uast::Function::DEFAULT_RETURN_INTENT:
    case uast::Function::OUT:
    case uast::Function::CONST:
      return RET_VALUE;
    case uast::Function::CONST_REF:
      return RET_CONST_REF;
    case uast::Function::REF:
      return RET_REF;
    case uast::Function::PARAM:
      return RET_PARAM;
    case uast::Function::TYPE:
      return RET_TYPE;
  }

  CHPL_UNIMPL("return intent case not handled");
  return RET_VALUE;
}

bool isAssignOp(UniqueString name) {
  return (name == USTR("=") ||
          name == USTR("+=") ||
          name == USTR("-=") ||
          name == USTR("*=") ||
          name == USTR("/=") ||
          name == USTR("%=") ||
          name == USTR("**=") ||
          name == USTR("&=") ||
          name == USTR("|=") ||
          name == USTR("^=") ||
          name == USTR(">>=") ||
          name == USTR("<<="));
}

const char* createAnonymousRoutineName(const uast::Function* node) {
  std::ignore = node;

  static const int maxDigits = 100;
  static unsigned int nextId = 0;
  static const char* prefix = "chpl_anon";
  char buf[maxDigits];

  if ((nextId + 1) == 0) INT_FATAL("Overflow for lambda ID number");

  auto kind = astr(uast::Function::kindToString(node->kind()));

  // Use sprintf to prevent buffer overflow if there are too many lambdas.
  int n = snprintf(buf, (size_t) maxDigits, "%s_%s_%i", prefix, kind,
                   nextId++);
  if (n > (int) maxDigits) INT_FATAL("Too many lambdas.");

  auto ret = astr(buf);
  return ret;
}

const char* convertFunctionNameAndAstr(const uast::Function* node) {
  auto name = node->name();
  auto kind = node->kind();

  if (node->isAnonymous()) return createAnonymousRoutineName(node);

  if (name.isEmpty()) {
    INT_ASSERT(kind == uast::Function::PROC);
    return nullptr;
  }

  const char* ret = nullptr;
  if (name == USTR("by")) {
    ret = "chpl_by";
  } else if (name == USTR("align")) {
    ret = "chpl_align";
  } else {
    ret = name.c_str();
  }

  INT_ASSERT(ret);

  // We have to uniquify the name now because it may be inlined (and thus
  // stack allocated).
  ret = astr(ret);

  return ret;
}

// build up the userString as in old parser
// needed to match up some error outputs
// NOTE: parentheses may have been discarded from the original user
// declaration, and if so, we are not able to reconstruct them at
// this time
const char* constructUserString(const uast::Function* node) {
  std::stringstream ss;
  printFunctionSignature(ss, node);
  auto ret = astr(ss.str());
  return ret;
}

const char* constructUserString(const uast::FunctionSignature* node) {
  std::stringstream ss;
  printFunctionSignature(ss, node);
  auto ret = astr(ss.str());
  return ret;
}

IntentTag convertFormalIntent(uast::Formal::Intent intent) {
  switch (intent) {
    case uast::Formal::DEFAULT_INTENT:
      return INTENT_BLANK;
    case uast::Formal::CONST:
      return INTENT_CONST;
    case uast::Formal::CONST_REF:
      return INTENT_CONST_REF;
    case uast::Formal::REF:
      return INTENT_REF;
    case uast::Formal::IN:
      return INTENT_IN;
    case uast::Formal::CONST_IN:
      return INTENT_CONST_IN;
    case uast::Formal::OUT:
      return INTENT_OUT;
    case uast::Formal::INOUT:
      return INTENT_INOUT;
    case uast::Formal::PARAM:
      return INTENT_PARAM;
    case uast::Formal::TYPE:
      return INTENT_TYPE;
  }

  CHPL_UNIMPL("Unhandled formal intent");
  return INTENT_BLANK;
}


ShadowVarPrefix convertTaskVarIntent(const uast::TaskVar* node) {
  // TODO: why is this here? It can't possible matter, here
  astlocMarker markAstLoc(node->id());

  switch (node->intent()) {
    case uast::TaskVar::VAR: return SVP_VAR;
    case uast::TaskVar::CONST: return SVP_CONST;
    case uast::TaskVar::CONST_REF: return SVP_CONST_REF;
    case uast::TaskVar::REF: return SVP_REF;
    case uast::TaskVar::IN: return SVP_IN;
    case uast::TaskVar::CONST_IN: return SVP_CONST_IN;
  }

  INT_FATAL("Should not reach here");
  return SVP_VAR;
}

const char* sanitizeVarName(const char* name, bool inTupleDecl) {
  if (inTupleDecl && name[0] == '_' && name[1] == '\0')
    return astr("chpl__tuple_blank");
  else
    return astr(name);
}

void attachSymbolStorage(const uast::Variable::Kind kind, Symbol* vs, bool setQual) {
  return attachSymbolStorage((uast::Qualifier) kind, vs, setQual);
}

void attachSymbolStorage(const uast::TupleDecl::IntentOrKind iok,
                         Symbol* vs, bool setQual) {
  return attachSymbolStorage((uast::Qualifier) iok, vs, setQual);
}

void attachSymbolStorage(const uast::Qualifier kind, Symbol* vs, bool setQual) {
  auto qual = QUAL_UNKNOWN;

  switch (kind) {
    case uast::Qualifier::VAR:
      qual = QUAL_VAL;
      break;
    case uast::Qualifier::CONST_VAR:
      vs->addFlag(FLAG_CONST);
      qual = QUAL_CONST;
      break;
    case uast::Qualifier::CONST_REF:
      vs->addFlag(FLAG_CONST);
      vs->addFlag(FLAG_REF_VAR);
      qual = QUAL_CONST_REF;
      break;
    case uast::Qualifier::REF:
      vs->addFlag(FLAG_REF_VAR);
      qual = QUAL_REF;
      break;
    case uast::Qualifier::PARAM:
      vs->addFlag(FLAG_PARAM);
      qual = QUAL_PARAM;
      break;
    case uast::Qualifier::TYPE:
      vs->addFlag(FLAG_TYPE_VARIABLE);
      break;
    case uast::Qualifier::INDEX:
      vs->addFlag(FLAG_INDEX_VAR);
      break;
    default:
      break;
  }

  if (setQual && qual != QUAL_UNKNOWN) {
    vs->qual = qual;
  }
}

bool isEnsureDomainExprCall(Expr* expr) {
  if (auto call = toCallExpr(expr)) {
    return call->isNamed("chpl__ensureDomainExpr");
  }
  return false;
}

AggregateTag convertAggregateDeclTag(const uast::AggregateDecl* node) {
  switch (node->tag()) {
    case uast::asttags::Class: return AGGREGATE_CLASS;
    case uast::asttags::Record: return AGGREGATE_RECORD;
    case uast::asttags::Union: return AGGREGATE_UNION;
    default: break;
  }
  INT_FATAL("Should not reach here!");
  return AGGREGATE_CLASS;
}

// compute the ModTag for a module stored at file 'path'
ModTag getModuleTag(Context* context, UniqueString path) {
  ModTag modTag = MOD_USER;
  if (chpl::parsing::filePathIsInInternalModule(context, path)) {
    modTag = MOD_INTERNAL;
  } else if (chpl::parsing::filePathIsInStandardModule(context, path)) {
    modTag = MOD_STANDARD;
  } else if (chpl::parsing::filePathIsInBundledModule(context, path)) {
    // TODO: this considers code in modules/packages as MOD_STANDARD but
    // we would like this to be MOD_USER.
    // See also issue #24998.
    modTag = MOD_STANDARD;
  }
  return modTag;
}
