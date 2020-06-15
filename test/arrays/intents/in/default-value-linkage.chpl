
var numErrors = 0;

proc main() {
  var D = {1..4};
  var Linked : [D] int;
  var Unlinked : [1..4] int;

  writeln("simple(D, Linked)");
  simple(D, Linked);
  writeln("simple(D, Unlinked)");
  simple(D, Unlinked);

  writeln("defaultsType()");
  defaultsType();
  writeln("defaultsType(D)");
  defaultsType(D);
  writeln("defaultsType(D, Linked)");
  defaultsType(D, Linked);
  writeln("defaultsType(D, Unlinked)");
  defaultsType(D, Unlinked);
  writeln("defaultsType(A=Linked)");
  defaultsType(A=Linked);
  writeln("defaultsType(A=Unlinked)");
  defaultsType(A=Unlinked);

  writeln("defaultsValue()");
  defaultsValue();
  writeln("defaultsValue(D)");
  defaultsValue(D);
  writeln("defaultsValue(D, Linked)");
  defaultsValue(D, Linked);
  writeln("defaultsValue(D, Unlinked)");
  defaultsValue(D, Unlinked);
  writeln("defaultsValue(A=Linked)");
  defaultsValue(A=Linked);
  writeln("defaultsValue(A=Unlinked)");
  defaultsValue(A=Unlinked);

  writeln("defaultFromArg(D, Linked)");
  defaultFromArg(D, Linked);
  writeln("defaultFromArg(D, Linked, Unlinked)");
  defaultFromArg(D, Linked, Unlinked);

  if numErrors > 0 {
    if numErrors == 1 then writeln("1 Failure");
    else writeln(numErrors, " Failures");
  } else {
    writeln("SUCCESS");
  }
}

proc check(ref D : domain, const ref A : []) {
  D = {1..D.size*2};
  if A.domain != D {
    numErrors += 1;
    writeln("ERROR:");
    writeln("  D        = ", D);
    writeln("  A.domain = ", A.domain);
    writeln("  A        = ", A);
  }
}

proc simple(in D : domain(1), in A : [D] int) {
  check(D, A);
}

proc defaultsType(in D : domain(1) = {1..4},
                  in A : [D] int = [1,2,3,4]) {
  check(D, A);
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
  check(D, B);
}
