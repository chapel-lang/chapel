class C {
  var a: int;
  var next: unmanaged C?;
}

var A: [1..5] unmanaged C?;

on Locales(1) {
  [i in A.domain] A(i) = new unmanaged C(i);
}

[i in A.domain] A(i)!.next = new unmanaged C(i+1);

var B = A!.next;
local {
  B!.next = new unmanaged C(B!.a + 1);
}

proc foo(c: unmanaged C?) {
  c!.a = c!.a - 3;
}

on Locales(1) {
  var localA = A; // Oops, localA still has remote domain data, etc.
  local {
    foo(localA); // error. See localBlock6.chpl for a workaround
  }
}
writeln(A);

