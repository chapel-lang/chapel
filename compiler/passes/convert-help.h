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

/* This file is a helper for the process of converting from uAST
   to the older AST used by the rest of the compiler.
   It contains details that can be shared by both convert-uast.cpp
   and convert-typed-uast.cpp.
   It helps to add LLVM attributes to certain expressions to support
   GPU programming.
 */

#ifndef _CONVERT_HELP_H_
#define _CONVERT_HELP_H_

#include "chpl/framework/Context.h"
#include "chpl/uast/Attribute.h"

#include "convert-uast.h"
#include "metadata.h"


using namespace chpl;

struct Converter;

struct LoopAttributeInfo {
 public:
  // LLVM metadata from various @llvm attributes.
  LLVMMetadataList llvmMetadata;
  // The @assertOnGpu attribute, if one is provided by the user.
  const uast::Attribute* assertOnGpuAttr = nullptr;
  // The @gpu.assertEligible attribute, which asserts GPU eligibility,
  // if one is provided by the user.
  const uast::Attribute* assertEligibleAttr = nullptr;
  // The @gpu.blockSize attribute, if one is provided by the user.
  const uast::Attribute* blockSizeAttr = nullptr;
  // The @gpu.itersPerThread attribute, if one is provided by the user.
  const uast::Attribute* itersPerThreadAttr = nullptr;

 private:
  LLVMMetadataPtr tupleToLLVMMetadata(Context* context,
                                      const uast::Tuple* node) const;

  LLVMMetadataPtr nodeToLLVMMetadata(Context* context,
                                     const uast::AstNode* node) const;
  LLVMMetadataList buildLLVMMetadataList(Context* context,
                                         const uast::Attribute* node) const;
  LLVMMetadataPtr buildAssertVectorize(const uast::Attribute* node) const;
  void readLlvmAttributes(Context* context,
                          const uast::AttributeGroup* attrs);
  void readNativeGpuAttributes(const uast::AttributeGroup* attrs);

 public:
  static LoopAttributeInfo fromExplicitLoop(Context* context,
                                            const uast::Loop* node) {
    auto attrs = node->attributeGroup();
    if (attrs == nullptr) return {};

    LoopAttributeInfo into;
    into.readLlvmAttributes(context, attrs);
    into.readNativeGpuAttributes(attrs);

    return into;
  }

  static LoopAttributeInfo fromVariableDeclaration(Context* context,
                                                   const uast::Variable* node) {
    auto attrs = node->attributeGroup();
    if (attrs == nullptr) return {};

    // Do not bother parsing LLVM attributes, since they don't apply to loops.
    LoopAttributeInfo into;
    into.readNativeGpuAttributes(attrs);

    return into;
  }

  bool empty() const {
    return llvmMetadata.size() == 0 &&
           assertOnGpuAttr == nullptr &&
           assertEligibleAttr == nullptr &&
           itersPerThreadAttr == nullptr &&
           blockSizeAttr == nullptr;
  }

  operator bool() const {
    return !empty();
  }

  bool insertGpuEligibilityAssertion(BlockStmt* body);
  bool insertBlockSizeCall(UastConverter& converter, BlockStmt* body);
  bool insertItersPerThreadCall(UastConverter& converter, BlockStmt* body);
  BlockStmt* createPrimitivesBlock(UastConverter& converter);
  void insertPrimitivesBlockAtHead(UastConverter& converter, BlockStmt* body);
};

#endif
