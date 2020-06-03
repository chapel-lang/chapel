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
writeln("First local block");
local {
  B!.next = new unmanaged C(B!.a + 1);
}

proc foo(c: unmanaged C?) {
  c!.a = c!.a - 3;
}

on Locales(1) {
  var localA: [A.domain.low..A.domain.high] unmanaged C?;
  localA = A;
writeln("Second local block");
  local {
    foo(localA);
  }
}


foo(A);
writeln("Third local block");
local {
  serial do foo(A); // this line causes the error
}

writeln(A);

