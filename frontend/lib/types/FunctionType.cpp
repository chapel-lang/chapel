/*
 * Copyright 2021-2024 Hewlett Packard Enterprise Development LP
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

#include "chpl/types/FunctionType.h"
#include "chpl/framework/query-impl.h"
#include "chpl/uast/AstNode.h"
#include "chpl/uast/Function.h"

#include <vector>

namespace chpl {
namespace types {

using namespace resolution;

const owned<FunctionType>&
FunctionType::getFunctionType(Context* context,
                              const TypedFnSignature* signature,
                              QualifiedType returnTypeAndIntent) {
  QUERY_BEGIN(getFunctionType, context, signature, returnTypeAndIntent);

  auto ufs = signature->untyped();

  // TODO: Not sure what this would even mean...
  CHPL_ASSERT(!ufs->isTypeConstructor());

  std::vector<UntypedFnSignature::FormalDetail> ufsFormals;
  for (int i = 0; i < ufs->numFormals(); i++) {
    UntypedFnSignature::FormalDetail f(ufs->formalName(i),
                                       ufs->formalHasDefault(i),
                                       /*decl*/ nullptr,
                                       ufs->formalIsVarArgs(i));
    ufsFormals.push_back(std::move(f));
  }

  auto newUfs = UntypedFnSignature::get(context,
                                        /*id*/ ID(),
                                        /*name*/ UniqueString(),
                                        ufs->isMethod(),
                                        ufs->isTypeConstructor(),
                                        /*isCompilerGenerated*/ true,
                                        ufs->throws(),
                                        ufs->idTag(),
                                        ufs->kind(),
                                        std::move(ufsFormals),
                                        /*whereClause*/ nullptr);

  std::vector<QualifiedType> formalTypes;
  for (int i = 0; i < signature->numFormals(); i++) {
    formalTypes.push_back(signature->formalType(i));
  }

  auto newTfs = TypedFnSignature::get(context, newUfs,
                                      std::move(formalTypes),
                                      TypedFnSignature::WHERE_NONE,
                                      signature->needsInstantiation(),
                                      /*instantiatedFrom*/ nullptr,
                                      /*parentFn*/ nullptr,
                                      /*outerVariableTypes*/ {},
                                      signature->formalsInstantiatedBitmap());

  // TODO: Sanitize the return type.
  auto ret = toOwned(new FunctionType(newTfs, returnTypeAndIntent));

  return QUERY_END(ret);
}

const FunctionType*
FunctionType::get(Context* context, const TypedFnSignature* signature,
                  QualifiedType returnTypeAndIntent) {
  return getFunctionType(context, signature, returnTypeAndIntent).get();
}


} // end namespace types
} // end namespace chpl
