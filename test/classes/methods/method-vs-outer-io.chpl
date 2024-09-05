// from issue #25551

use Sort;
use IO;

proc foo(a, i: integral, const text) {
  if i >= text.size then
    return (keyPartStatus.pre, 0);

  return (keyPartStatus.returned, text[i]);
}

proc main() {
  var text: [0..10] int; // renaming this variable fixes the problem
  record myComparator : keyPartComparator {
    proc keyPart(a, i) {
      return foo(a, i=i, text=text);
      // there was a bug where 'text' above resolved to a method in IO
    }
  }

  var A:[1..10] int = 1..10;
  sort(A, new myComparator());
}
