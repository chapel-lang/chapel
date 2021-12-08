use Sort;

import Reflection.canResolveMethod;
record ContrivedComparator {
  const dc = new DefaultComparator();
  proc keyPart(a, i: int) {
    // Fall back to DefaultComparator implementation where possible
    if canResolveMethod(dc, "keyPart", a, 0) {
      return dc.keyPart(a, i);
    } else if isTuple(a) {
      // DefaultComparator does not handle non-homogeneous tuples
      return tupleKeyPart(a, i);
    } else {
      compilerError("No keyPart method for eltType ", a.type:string);
    }
  }

  proc tupleKeyPart(x, i:int) {
    // This is the same as DefaultComparator.keyPart() for homogeneous tuple
    const tup = dc.keyPart(x(i), 1);
    const part = tup(1);
//    const (_, part) = dc.keyPart(x(i), 1);
    if i >= x.size {
      return (-1, 0:part.type);
    } else {
      return (0, part);
    }
  }
}

const myDefaultComparator = new ContrivedComparator();

var A: [0..#10] (real, int) = for i in 0..#10 do ((10-i):real, i);
// Using the twoArrayRadixSort generates internal error
TwoArrayRadixSort.twoArrayRadixSort(A, comparator=myDefaultComparator);
// Using the regular sort instead does not
sort(A);
