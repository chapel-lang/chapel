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


