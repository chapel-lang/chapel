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

#include "resolution-help.h"

namespace chpl {
namespace resolution {


using namespace uast;
using namespace types;

bool
anyFormalNeedsInstantiation(Context* context,
                            const std::vector<types::QualifiedType>& f,
                            const UntypedFnSignature* ufs,
                            SubstitutionsMap* subs) {
  bool genericOrUnknown = false;
  int i = 0;
  for (const auto& qt : f) {
    if (qt.isUnknown()) {
      genericOrUnknown = true;
      break;
    }

    bool considerGenericity = true;
    if (subs != nullptr) {
      auto formalDecl = ufs->formalDecl(i);
      if (subs->count(formalDecl->id())) {
        // don't consider it needing a substitution - e.g. when passing
        // a generic type into a type argument.
        considerGenericity = false;
      }
    }

    if (considerGenericity) {
      auto g = getTypeGenericity(context, qt);
      if (g != Type::CONCRETE) {
        genericOrUnknown = true;
        break;
      }
    }
    i++;
  }

  return genericOrUnknown;
}

} // end namespace resolution
} // end namespace chpl
