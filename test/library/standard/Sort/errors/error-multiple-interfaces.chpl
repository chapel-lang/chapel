use Sort;

config type comparator;

// TODO: when Sort.keyComparator is implemented, this should be removed
// the test will fail and notify us when that occurs
interface keyComparator { }

var A = [1,2,3,4];

record R1: keyPartComparator, relativeComparator {
  proc compare(a: int, b: int) {
    return a - b;
  }
}
record R2: keyPartComparator, keyComparator {
  proc key(a: int) {
    return a;
  }
}
record R3: relativeComparator, keyComparator {
  proc key(a: int) {
    return a;
  }
}
record R4: keyComparator, keyPartComparator, relativeComparator {
  proc key(a: int) {
    return a;
  }
}

sort(A, comparator=new comparator());
