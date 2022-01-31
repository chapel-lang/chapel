/*
 * Copyright 2021-2022 Hewlett Packard Enterprise Development LP
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

#include "chpl/resolution/disambiguation.h"

#include "chpl/parsing/parsing-queries.h"
#include "chpl/queries/query-impl.h"
#include "chpl/types/all-types.h"
#include "chpl/uast/Function.h"

namespace chpl {
namespace resolution {


using namespace uast;
using namespace types;

struct DisambiguationState {
  bool  fn1MoreSpecific = false;
  bool  fn2MoreSpecific = false;

  bool  fn1Promotes = false;
  bool  fn2Promotes = false;

  bool fn1WeakPreferred = false;
  bool fn2WeakPreferred = false;

  bool fn1WeakerPreferred = false;
  bool fn2WeakerPreferred = false;

  bool fn1WeakestPreferred = false;
  bool fn2WeakestPreferred = false;
};

#define ENABLE_TRACING_OF_DISAMBIGUATION 1

#ifdef ENABLE_TRACING_OF_DISAMBIGUATION

#define EXPLAIN(...) \
        if (explain) fprintf(stderr, __VA_ARGS__)

#else

#define EXPLAIN(...)

#endif

/*
  Find the most specific candidates and return them.

  Does not consider return intent overloading.
  */
static std::vector<const TypedFnSignature*>
findMostSpecific(Context* context,
                 std::vector<const TypedFnSignature*> lst,
                 CallInfo call,
                 bool ignoreWhere,
                 bool explain) {
  std::vector<const TypedFnSignature*> ret;

  if (lst.size() == 0) {
    // nothing to do
  } else if (lst.size() == 1) {
    ret.push_back(lst[0]);
  } else {
    // TODO: find most specific -- pull over disambiguation code
    // TODO: handle return intent overloading
    // TODO: this is demo code
    if (call.numActuals() > 1) {
      if (call.actuals(1).type().type()->isIntType()) {
        ret.push_back(lst[0]);
      } else {
        ret.push_back(lst[lst.size()-1]);
      }
    } else {
      ret.push_back(lst[0]);
    }
  }

  return ret;
}


const MostSpecificCandidates&
findMostSpecificCandidates(Context* context,
                           std::vector<const TypedFnSignature*> lst,
                           CallInfo call) {
  QUERY_BEGIN(findMostSpecificCandidates, context, lst, call);

  bool explain = false;
  MostSpecificCandidates result;

  if (lst.size() == 0) {
    // nothing to do, return no candidates
  } else if (lst.size() == 1) {
    // If there is just one candidate, return it
    result.setBestOnly(lst[0]);

  } else {
    // > 1 candidates
    auto ambiguous = findMostSpecific(context, lst, call, true, explain);

    if (ambiguous.size() == 0) {
      // nothing to do, return no candidates
    } else if (ambiguous.size() == 1) {
      // The common case is that there is no ambiguity because the
      // return intent overload feature is not used.
      result.setBestOnly(ambiguous[0]);

    } else {
      // Now, if there was ambiguity, try again while considering
      // separately each category of return intent.
      //
      // If there is only one most specific function in each category,
      // that is what we need to return.
      int                  nRef              = 0;
      int                  nConstRef         = 0;
      int                  nValue            = 0;
      int                  nOther            = 0;

      // Count number of candidates in each category.
      for (auto fn : ambiguous) {
        auto returnIntent = parsing::idToFnReturnIntent(context, fn->id());

        switch (returnIntent) {
          case Function::DEFAULT_RETURN_INTENT:
          case Function::CONST:
            nValue++;
            break;
          case Function::CONST_REF:
            nConstRef++;
            break;
          case Function::REF:
            nRef++;
            break;
          case Function::PARAM:
          case Function::TYPE:
            nOther++;
            break;
        }
      }

      if (nOther > 0) {
        // If there are *any* type/param candidates, we need to cause ambiguity
        // if they are not selected... including consideration of where clauses.
        ambiguous = findMostSpecific(context, lst, call, false, explain);
        if (ambiguous.size() == 1) {
          result.setBestOnly(ambiguous[0]);
        }
      } else if (nRef > 1 || nConstRef > 1 || nValue > 1) {
        // Split candidates into ref, const ref, and value candidates
        std::vector<const TypedFnSignature*> refCandidates;
        std::vector<const TypedFnSignature*> constRefCandidates;
        std::vector<const TypedFnSignature*> valueCandidates;

        // Gather the candidates in each category
        for (auto fn : ambiguous) {
          auto returnIntent = parsing::idToFnReturnIntent(context, fn->id());

          switch (returnIntent) {
            case Function::DEFAULT_RETURN_INTENT:
            case Function::CONST:
              valueCandidates.push_back(fn);
              break;
            case Function::CONST_REF:
              constRefCandidates.push_back(fn);
              break;
            case Function::REF:
              refCandidates.push_back(fn);
              break;
            case Function::PARAM:
            case Function::TYPE:
              break;
          }
        }

        // Disambiguate each group
        auto refs = findMostSpecific(context, refCandidates, call, false, explain);
        auto constRefs = findMostSpecific(context, constRefCandidates, call, false, explain);
        auto values = findMostSpecific(context, valueCandidates, call, false, explain);

        // update the counts
        nRef = refs.size();
        nConstRef = constRefs.size();
        nValue = values.size();

        // If there is 1 or fewer match in each category, OK
        if (nRef <= 1 && nConstRef <= 1 && nValue <= 1) {
          if (nRef == 1)
            result.setBestRef(refs[0]);
          if (nConstRef == 1)
            result.setBestConstRef(constRefs[0]);
          if (nValue == 1)
            result.setBestValue(values[0]);
        }
        // otherwise, fail to match due to ambiguity
      }
    }
  }

  return QUERY_END(result);
}


} // end namespace resolution
} // end namespace chpl
