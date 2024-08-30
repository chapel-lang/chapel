use Sort;

config type comparator;

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

var A = [1,2,3,4];
sort(A, comparator=new comparator());
