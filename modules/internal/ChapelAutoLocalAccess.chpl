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

module ChapelAutoLocalAccess {
  use ChapelLocale;

  // note that the compiler can pass an iterator to `loopDomain` argument. Make
  // sure that we don't do anything with iterators as we cannot optimize such
  // forall's and we don't want to mess up the iterator
  proc chpl__ala_staticCheck(accessBase: [], loopDomain: domain,
                             myIterand: domain, param hasOffsets=false) param {
    if hasOffsets && !accessBase.domain.supportsOffsetAutoLocalAccess() {
      return false;
    }

    if accessBase.domain.type == loopDomain.type {
      if chpl__isArrayViewWithDifferentDist(accessBase) {
        return false;
      }
      else {
        return loopDomain.supportsAutoLocalAccess();
      }
    }

    // support forall i in a.domain.localSubdomain() do .... a[i] ....
    if loopDomain._value.type.isDefaultRectangular() {
      return accessBase.domain.supportsAutoLocalAccess() &&
             accessBase.rank == loopDomain.rank;
    }

    return false;
  }

  proc chpl__ala_staticCheck(accessBase: [], loopDomain: [],
                             myIterand: domain, param hasOffsets=false) param {
    return chpl__ala_staticCheck(accessBase, loopDomain.domain, myIterand,
                                 hasOffsets);
  }

  proc chpl__ala_staticCheck(accessBase, loopDomain, myIterand,
                             param hasOffsets=false) param {
    return false;
  }

  // these type overloads are for degenerate cases where the optimization can
  // break a meaningful error message without these
  proc chpl__ala_staticCheck(type accessBase, type loopDomain,
                             param hasOffsets=false) param {
    return false;
  }
  proc chpl__ala_staticCheck(accessBase, type loopDomain,
                             param hasOffsets=false) param {
    return false;
  }
  proc chpl__ala_staticCheck(type accessBase, loopDomain,
                             param hasOffsets=false) param {
    return false;
  }

  proc chpl__ala_dynamicCheck(accessBase: [], loopDomain: domain,
                              myIterand: domain, param hasOffsets=false) {
    if chpl__ala_staticCheck(accessBase, loopDomain, hasOffsets) {
      // if they're the same domain...
      if chpl_sameDomainKind(accessBase.domain, loopDomain) &&
         accessBase.domain == loopDomain                    &&
         accessBase.domain._value.dist.dsiEqualDMaps(loopDomain._value.dist) then
        return true;

      // or at least if they were distributed the same way
      if accessBase.domain.distribution == loopDomain.distribution then return true;

      // if we are iterating over a rectangular that's:
      // 1. not remote
      // 2. falls within the localSubdomain of the array
      //
      // Be also aware that `subset` call below can be expensive if we are not
      // calling on default rectangular
      if myIterand._value.type.isDefaultRectangular() {
        if myIterand.locale == here {
          if accessBase.localSubdomain().contains(myIterand) {
            return true;
          }
        }
      }
    }

    return false;
  }

  /*inline proc chpl__ala_dynamicCheck(accessBase: [], loopDomain: [],*/
                                     /*param hasOffsets=false) {*/
    /*return chpl__ala_dynamicCheck(accessBase, loopDomain.domain, hasOffsets);*/
  /*}*/

  // these type overloads are for degenerate cases where the optimization can
  // break a meaningful error message without these
  proc chpl__ala_dynamicCheck(accessBase, loopDomain, myIterand,
                              param hasOffsets=false) {
    return false;
  }
  proc chpl__ala_dynamicCheck(type accessBase, type loopDomain,
                              param hasOffsets=false) {
    return false;
  }
  proc chpl__ala_dynamicCheck(accessBase, type loopDomain,
                              param hasOffsets=false) {
    return false;
  }
  proc chpl__ala_dynamicCheck(type accessBase, loopDomain,
                              param hasOffsets=false) {
    return false;
  }

  inline proc chpl__ala_offsetCheck(accessBase: [], offsets:integral...) {
    if (offsets.size != accessBase.rank) {
      compilerError("Automatic local access optimization failure: ",
                    "Number of offsets doesn't match rank");
    }
    return accessBase.domain.autoLocalAccessOffsetCheck(offsets);
  }

  // what if the user had `MyArr[i+"1"]` in their code? We don't want to see
  // resolution errors coming from this function. That code should error out
  // later in compilation with a proper error message
  inline proc chpl__ala_offsetCheck(accessBase, offsets...) {
    return false;
  }

  proc chpl__isArrayViewWithDifferentDist(arr: []) param {
    // Slices can have different distributions than the original array which can
    // cause false optimizations
    if arr._value.isSliceArrayView() {
      return chpl__getActualArray(arr._value).dom.type != arr.domain._value.type;
    }
    else {
      // Non-slice views have different domain types, but they do not impact how
      // the values are distributed. So, they are safer from ALA standpoint
      return false;
    }

  }
}
