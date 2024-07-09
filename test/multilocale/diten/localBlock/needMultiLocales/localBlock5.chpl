class C {
  var a: int;
  var next: shared C?;
}

var A: [1..5] shared C?;

on Locales(1) {
  [i in A.domain] A(i) = new shared C(i);
}

[i in A.domain] A(i)!.next = new shared C(i+1);

var B = A!.next;
local {
  B!.next = new shared C(B!.a + 1);
}

proc foo(c: C?) {
  c!.a = c!.a - 3;
}

on Locales(1) {
  var localA = A; // Oops, localA still has remote domain data, etc.
  local {
    foo(localA); // error. See localBlock6.chpl for a workaround
  }
}
writeln(A);

