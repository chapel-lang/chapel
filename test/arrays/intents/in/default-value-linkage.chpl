
var numErrors = 0;

proc main() {
  var D = {1..4};
  var Linked : [D] int;
  var Unlinked : [1..4] int;

  simple(D, Linked);
  simple(D, Unlinked);

  defaultsType();
  defaultsType(D);
  defaultsType(D, Linked);
  defaultsType(D, Unlinked);
  defaultsType(A=Linked);
  defaultsType(A=Unlinked);

  defaultsValue();
  defaultsValue(D);
  defaultsValue(D, Linked);
  defaultsValue(D, Unlinked);
  defaultsValue(A=Linked);
  defaultsValue(A=Unlinked);

  defaultFromArg(D, Linked);
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
