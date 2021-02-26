var DD = {1..4};

proc main() {
  var Linked : [DD] int;
  var Unlinked : [1..4] int;

  writeln("defaultsValue(D, Linked)");
  defaultsValue(DD, Linked);
  writeln();

  writeln("defaultsValue(D, Unlinked)");
  defaultsValue(DD, Unlinked);
  writeln();

  writeln("defaultsValue(A=Linked)");
  defaultsValue(A=Linked);
  writeln();

  writeln("defaultsValue(A=Unlinked)");
  defaultsValue(A=Unlinked);
  writeln();

  writeln("defaultFromArg(D, Linked, Unlinked)");
  defaultFromArg(DD, Linked, Unlinked);
  writeln();
}

proc check(ref D : domain, const ref A : []) {
  writeln("A.domain is ", A.domain);
  writeln("Modifying D");
  D = {1..5};
  writeln("A.domain is now ", A.domain);
  D = {1..4}; // put it back

  writeln("Modifying DD");
  DD = {1..6};
  writeln("A.domain is now ", A.domain);
  DD = {1..4}; // put it back
}

proc builder(D : domain(1)) {
  var R : [D] int;
  return R;
}

proc defaultsValue(in D = {1..4},
                   in A = builder(D)) {
  check(D, A);
}

proc defaultFromArg(in D : domain(1), in A : [D] int, in B = A) {
  writeln("  checking A");
  check(D, A);
  writeln("  checking B");
  check(D, B);
}
