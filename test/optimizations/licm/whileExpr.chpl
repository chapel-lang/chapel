record Box {
  var count = 10;
}

var A : [1..1] Box;

proc helper(i) {
  A[i].count -= 1;
}

proc globalVersion() {
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

proc nonGlobalVersion() {
  var B : [1..1] Box;

  proc helper(i) { B[i].count -= 1; }
  ref box = B[1];
  var BI = 1;

  var i = 1;
  // 'box.count' should not be hoisted!
  while i <= box.count {
    writeln("ITER ", i);
    helper(BI);
    i += 1;
  }
}

proc inlinedNonGlobalVersion() {
  var B = [10];
  ref box = B[0];

  var i = 1;
  while i <= box {
    writeln("ITER ", i);
    B[0] -= 1;
    i += 1;
  }
}

proc main() {
  globalVersion();
  writeln();
  nonGlobalVersion();
  writeln();
  inlinedNonGlobalVersion();
}
