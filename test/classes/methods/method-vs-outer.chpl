// from issue #25551

module Unrelated {
  record Unrelated { }
  proc Unrelated.text() { } // problem goes away if this is a primary method

  proc mySort(A: [], comparator) {
    comparator.keyPart(A.first, 0);
  }
}

module Main {
  use Unrelated;

  proc foo(a, i: integral, const text) {
    if i >= text.size then
      return (-1, 0);

    return (0, text[i]);
  }

  proc main() {
    var text: [0..10] int; // renaming this variable fixes the problem
    record myComparator {
      proc keyPart(a, i) {
        return foo(a, i=i, text=text);
      }
    }

    var A:[1..10] int = 1..10;
    mySort(A, new myComparator());
  }
} 
