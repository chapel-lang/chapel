/*
 * Copyright 2021 Hewlett Packard Enterprise Development LP
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

#include "chpl/AST/ASTTypes.h"

#include "chpl/AST/BaseAST.h"

namespace chpl {
namespace ast {


bool combineASTLists(ASTList& keep, ASTList& addin) {
  size_t keepSize = keep.size();
  size_t addinSize = addin.size();
  size_t keepIdx = 0;
  size_t addinIdx = 0;
  bool allMatched = true;

  // handle some common short scenarios directly
  if (keepSize == 0 && addinSize == 0) {
    return true;
  } else if (keepSize == 1 && addinSize == 0) {
    keep.swap(addin);
    return false;
  } else if (keepSize == 0 && addinSize == 1) {
    keep.swap(addin);
    return false;
  } else if (keepSize == 1 && addinSize == 1) {
    return BaseAST::combineAST(keep[0], addin[0]);
  }

  ASTList newList;
  ASTList junkList;

  /*
  printf("BEGINNING combineASTLists\n");
  printf("keep list: \n");
  for (auto & elt : keep) {
    BaseAST::dump(elt.get());
  }
  printf("addin list: \n");
  for (auto & elt : addin) {
    BaseAST::dump(elt.get());
  }
   */

  // Append the elements from addin to newList, but
  // if we find an existing element that matches,
  // keep that.

  // checks for:
  //   insertion
  //   deletion
  //   replacement
  while (addinIdx < addinSize) {
    /*
    printf("\n");
    printf("in loop next keeps \n");
    if (keepIdx < keepSize)
      BaseAST::dump(keep[keepIdx].get());
    if (keepIdx+1 < keepSize)
      BaseAST::dump(keep[keepIdx+1].get());
    printf("in loop next addins \n");
    if (addinIdx < addinSize)
      BaseAST::dump(addin[addinIdx].get());
    if (addinIdx+1 < addinSize)
      BaseAST::dump(addin[addinIdx+1].get());
     */

    bool eltMatched = false;
    if (keepIdx < keepSize &&
        keep[keepIdx]->shallowMatch(addin[addinIdx].get())) {
      owned<BaseAST> keepElt;
      owned<BaseAST> addinElt;
      keepElt.swap(keep[keepIdx]);
      addinElt.swap(addin[addinIdx]);
      // it seems like a close enough match, so combine it
      eltMatched = BaseAST::combineAST(keepElt, addinElt);
      // combineAST might have swapped the elements but
      // now keepElt is the one to put in newList.
      newList.push_back(std::move(keepElt));
      // put the other thing in junkList
      junkList.push_back(std::move(addinElt));
      addinIdx++;
      keepIdx++;
    } else if (addinIdx+1 < addinSize &&
               keep[keepIdx]->shallowMatch(addin[addinIdx+1].get())) {
      // like an element was added in addin,
      // so add two elements from addin and pass 1 keep element
      owned<BaseAST> keepElt;
      owned<BaseAST> addinEltOne;
      owned<BaseAST> addinEltTwo;
      keepElt.swap(keep[keepIdx]);
      addinEltOne.swap(addin[addinIdx]);
      addinEltTwo.swap(addin[addinIdx+1]);

      // keepElt matched addinEltTwo so try to combine them
      BaseAST::combineAST(keepElt, addinEltTwo);
      // now keepElt is the one to keep and addinEltTwo is junk
      newList.push_back(std::move(addinEltOne));
      newList.push_back(std::move(keepElt));
      junkList.push_back(std::move(addinEltTwo));
      addinIdx += 2;
      keepIdx++;
    } else if (keepIdx+1 < keepSize &&
               keep[keepIdx+1]->shallowMatch(addin[addinIdx].get())) {
      // like an element was deleted from keep,
      // so add an element from addin but pass 2 keep elements
      owned<BaseAST> addinElt;
      owned<BaseAST> keepEltOne;
      owned<BaseAST> keepEltTwo;
      addinElt.swap(addin[addinIdx]);
      keepEltOne.swap(keep[keepIdx]);
      keepEltTwo.swap(keep[keepIdx+1]);

      // keepEltTwo matched addinElt so try to combine them
      BaseAST::combineAST(keepEltTwo, addinElt);
      // now keepEltTwo is the one to keep and addinElt is junk
      newList.push_back(std::move(keepEltTwo));
      junkList.push_back(std::move(keepEltOne));
      junkList.push_back(std::move(addinElt));
      addinIdx++;
      keepIdx += 2;
    } else if (addinIdx+1 < addinSize && keepIdx+1 < keepSize &&
               keep[keepIdx+1]->shallowMatch(addin[addinIdx+1].get())) {
      // like an element was replaced from keep,
      // so add 2 elements from addin and pass 2 keep elements
      owned<BaseAST> addinEltOne;
      owned<BaseAST> addinEltTwo;
      owned<BaseAST> keepEltOne;
      owned<BaseAST> keepEltTwo;
      addinEltOne.swap(addin[addinIdx]);
      addinEltTwo.swap(addin[addinIdx+1]);
      keepEltOne.swap(keep[keepIdx]);
      keepEltTwo.swap(keep[keepIdx+1]);

      // keepEltTwo matched addinEltTwo so try to combine them
      BaseAST::combineAST(keepEltTwo, addinEltTwo);
      // now keepEltTwo is the one to keep and addinEltTwo is junk
      newList.push_back(std::move(addinEltOne));
      newList.push_back(std::move(keepEltTwo));
      junkList.push_back(std::move(keepEltOne));
      junkList.push_back(std::move(addinEltTwo));
      addinIdx += 2;
      keepIdx += 2;
    } else {
      // give up on trying to match things
      allMatched = false;
      break;
    }

    allMatched = allMatched && eltMatched;
  }

  // if we gave up trying to match things, we end up in this loop
  for (; addinIdx < addinSize; addinIdx++) {
    owned<BaseAST> addinElt;
    addinElt.swap(addin[addinIdx]);
    newList.push_back(std::move(addinElt));
  }
  for (; keepIdx < keepSize; keepIdx++) {
    owned<BaseAST> keepElt;
    keepElt.swap(keep[keepIdx]);
    junkList.push_back(std::move(keepElt));
  }

  assert(newList.size() == addin.size());
  // Swap the lists into place.
  keep.swap(newList);
  addin.swap(junkList);

  /*
  printf("ENDING combineASTLists\n");
  printf("keep list (result): \n");
  for (auto & elt : keep) {
    BaseAST::dump(elt.get());
  }
  printf("addin list (discard): \n");
  for (auto & elt : addin) {
    BaseAST::dump(elt.get());
  }
   */

  return allMatched;
}


} // end namespace ast
} // end namespace chpl
