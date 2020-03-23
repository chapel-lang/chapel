
use BlockDist;
use PrivateDist;
use Random;

config const n = 100;

proc main() {
  testType(int);
  testType(real);
  testType(bool);
}

proc testType(type T) {
  var D = {1..n};
  var S = D dmapped Block(D);

  var DR : [D] T;
  fillRandom(DR);
  test(DR);

  var BL : [S] T = DR;
  test(BL);

  var PR : [PrivateSpace] T = DR[1..numLocales];
  test(PR);

}

proc test(A : []) {
  writeln("Testing type '", A.type:string, "'");

  if isNumericType(A.eltType) {
    {
      writeln("+ scan");
      const expected = + scan serialize(A);
      check(+ scan A, expected);
    }
    {
      writeln("min scan");
      const expected = min scan serialize(A);
      check(min scan A, expected);
    }
    {
      writeln("max scan");
      const expected = max scan serialize(A);
      check(max scan A, expected);
    }
    {
      writeln("* scan");
      const expected = * scan serialize(A);
      check(* scan A, expected);
    }
    {
      writeln("maxloc scan");
      const expected = maxloc scan zip(serialize(A), A.domain);
      check(maxloc scan zip(A, A.domain), expected);
    }
    {
      writeln("minloc scan");
      const expected = minloc scan zip(serialize(A), A.domain);
      check(minloc scan zip(A, A.domain), expected);
    }

    if (isIntegralType(A.eltType)) {
      {
        writeln("& scan");
        const expected = & scan serialize(A);
        check(& scan A, expected);
      }
      {
        writeln("| scan");
        const expected = | scan serialize(A);
        check(| scan A, expected);
      }
      {
        writeln("^ scan");
        const expected = ^ scan serialize(A);
        check(^ scan A, expected);
      }
    }
  } else if isBool(A.eltType) {
    {
      writeln("&& scan");
      const expected = && scan serialize(A);
      check(&& scan A, expected);
    }
    {
      writeln("|| scan");
      const expected = || scan serialize(A);
      check(|| scan A, expected);
    }
  } else {
    compilerError("Unsupported element type '", A.eltType:string, "'");
  }

  writeln();
}

iter serialize(A : []) {
  for a in A do yield a;
}

proc check(A : [], E : []) {
  var equal = false;
  if isTuple(A.eltType) {
    equal   = && reduce [(a, e) in zip(A, E)] almostEquals(a(1), e(1));
    equal &&= && reduce [(a, e) in zip(A, E)] almostEquals(a(2), e(2));
  } else {
    equal = && reduce almostEquals(A, E);
  }

  if !equal {
    writeln("  Results for ", A.type:string, " do not match expected results");
    writeln("  Received = ", A);
    writeln("  Expected = ", E);
  } else {
    writeln("  Success");
  }
}

proc almostEquals(a, b, significant=6) {
  return abs(b - a) <= 10.0**-(significant-1);
}
