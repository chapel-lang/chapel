use Sort;

config type comparator;

// TODO: when Sort.keyComparator is implemented, this should be removed
// the test will fail and notify us when that occurs
interface keyComparator { }

var A = [1,2,3,4];

record R1: keyPartComparator, relativeComparator { }
record R2: keyPartComparator, keyComparator { }
record R3: relativeComparator, keyComparator { }
record R4: keyComparator, keyPartComparator, relativeComparator { }

sort(A, comparator=new comparator());
