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

  ASTList newList;

  /*
  printf("BEGINNING combineASTLists\n");
  printf("keep list: \n");
  for (auto & elt : keep) {
    BaseAST::dump(elt.get());
  }
  printf("addin list: \n");
  for (auto & elt : addin) {
    BaseAST::dump(elt.get());
  }*/

  // Append the elements from addin to newList, but
  // if we find an existing element that matches,
  // keep that.

  // checks for:
  //   insertion
  //   deletion
  //   replacement
  while (addinIdx < addinSize) {
    bool eltMatched = false;
    if (keepIdx < keepSize &&
        keep[keepIdx]->shallowMatch(addin[addinIdx].get())) {
      owned<BaseAST> keepElt;
      owned<BaseAST> addinElt;
      keepElt.swap(keep[keepIdx]);
      addinElt.swap(addin[addinIdx]);
      // it seems like a close enough match, so combine it
      eltMatched = BaseAST::combineAST(keepElt, addinElt);
      // now keepElt has the thing to return
      newList.push_back(std::move(keepElt));
      keepIdx++;
      addinIdx++;
    } else if (addinIdx+1 < addinSize &&
               keep[keepIdx]->shallowMatch(addin[addinIdx+1].get())) {
      // like an element was added in addin,
      // so add two elements from addin and pass 1 keep element
      owned<BaseAST> keepEltOne;
      owned<BaseAST> keepEltTwo;
      keepEltOne.swap(keep[keepIdx]);
      keepEltTwo.swap(keep[keepIdx+1]);
      newList.push_back(std::move(keepEltOne));
      newList.push_back(std::move(keepEltTwo));
      keepIdx++;
    } else if (keepIdx+1 < keepSize &&
               keep[keepIdx+1]->shallowMatch(addin[addinIdx].get())) {
      // like an element was deleted from keep,
      // so add an element from addin but pass 2 keep elements
      owned<BaseAST> keepElt;
      keepElt.swap(keep[keepIdx]);
      newList.push_back(std::move(keepElt));
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

  assert(newList.size() == addin.size());
  newList.swap(keep);

  /*
  printf("ENDING combineASTLists\n");
  printf("keep list (result): \n");
  for (auto & elt : keep) {
    BaseAST::dump(elt.get());
  }
  printf("addin list (discard): \n");
  for (auto & elt : addin) {
    BaseAST::dump(elt.get());
  }*/

  return allMatched;
}


} // end namespace ast
} // end namespace chpl
