
record Box {
  var count = 10;
}

var A : [1..1] Box;

proc helper(i) {
  A[i].count -= 1;
}

proc main() {
  for (box, AI) in zip(A, A.domain) {
    var i = 1;
    // 'box.count' should not be hoisted!
    while i <= box.count {
      writeln("ITER ", i);
      helper(AI);
      i += 1;
    }
  }
}
