class C {
  var a: int;
  var next: C;
}

var A: [1..5] unmanaged C;

on Locales(1) {
  [i in A.domain] A(i) = new unmanaged C(i);
}

[i in A.domain] A(i).next = new unmanaged C(i+1);

var B = A.next;
local {
  B.next = new unmanaged C(B.a + 1);
}

proc foo(c: C) {
  c.a = c.a - 3;
}

on Locales(1) {
  var localA: [A.domain.low..A.domain.high] C;
  localA = A;
  local {
    foo(localA);
  }
}
writeln(A);

