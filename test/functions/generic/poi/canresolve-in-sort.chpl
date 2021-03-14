/*
This code illustrates that canResolve() et al.
needs the functions visible to its caller.

Simplified from
  library/packages/Search/correctness.chpl

in particular, this code:
  use Search;
  proc main() {
    const A = [-4, -1, 2, 3];
    search(A, 2);
  }
*/
module Application {
  use MySort;

  proc main() {
    BinarySearch(1, 2, defaultComparator);
  }

  // originally: in Search module
  proc BinarySearch(Data, val, comparator) {
    chpl_compare(Data, val, comparator=comparator);
  }
}

module MySort {
  const defaultComparator: DefaultComparator;

  proc chpl_compare(a, b, comparator) {
    use MyReflection;
    if canResolveMethod(comparator, "compare", a, b) {
      comparator.compare(a ,b); // here, proc compare is visible without POI
    }
  }

  record DefaultComparator {
    proc compare(a, b) {
      compilerError("success");
    }
  }
}

module MyReflection {
  proc canResolveMethod(recv, name, a, b) param {
    recv.compare(a,b);  // originally: PRIM_METHOD_CALL_AND_FN_RESOLVES
    return true;
  }
}
