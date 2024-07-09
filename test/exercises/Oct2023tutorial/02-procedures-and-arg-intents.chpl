// Example to compute the area of a circle
proc area(radius: real): real {
  return 3.14 * radius**2;
}

writeln(area(2.0));  // 12.56

proc area_with_type_inference(radius) {
  return 3.14 * radius**2;
}

proc writeCoord(x: real = 0.0, y: real = 0.0) {
  writeln((x,y));
}

// Example of argument default values, naming
writeCoord(2.0);        // (2.0, 0.0)
writeCoord(y=2.0);      // (0.0, 2.0)
writeCoord(y=2.0, 3.0); // (3.0, 2.0)

// arguments can optionally be given intents
{
  proc foo(x: real, y: [] real) {
    // x = 1.2;  // illegal: scalars are passed ‘const in’ by default
    // y = 3.4;  // illegal: 'ref' by default for arrays is deprecated
  }

  var r: real,
      A: [1..3] real;

  foo(r, A);

  writeln((r, A));  // writes (0.0, [0.0, 0.0, 0.0])

}
{
  proc foo(ref x: real, ref y: [] real) {
    x = 1.2;  // OK: actual is modified
    y = 3.4;  // OK: actual is modified
  }

  var r: real,
      A: [1..3] real;

  foo(r, A);

  writeln((r, A));  // writes (1.2, [3.4, 3.4, 3.4])

}
{
  proc foo(ref x: real, ref y: [] real) {
    x = 1.2;  // OK: actual is modified
    y = 3.4;  // OK: actual is modified
  }

  const r: real,
        A: [1..3] real;

  // foo(r, A);  // illegal, can’t pass references to constants

  writeln((r, A));  // writes (0.0, [0.0, 0.0, 0.0])
}
{
  proc foo(const ref x: real, const ref y: [] real) {
    // x = 1.2;  // illegal: can’t modify constant arguments
    // y = 3.4;  // illegal: can’t modify constant arguments
  }

  const r: real,
        A: [1..3] real;

  foo(r, A);  // OK to create constant references to constants

  writeln((r, A));  // writes (0.0, [0.0, 0.0, 0.0])
}
{
  proc foo(param x: real, type t) {
    writeln("x = ", x);
  }

  const r: real,
        A: [1..3] real;

  // foo(r, A);  // illegal: can’t pass vars and consts to params and types

  writeln((r, A));  // writes (0.0, [0.0, 0.0, 0.0])
}
{
  proc foo(param x: real, type t) {
    writeln("x = ", x);
  }

  const r: real,
        A: [1..3] real;

  foo(1.2, r.type);  // OK: passing a literal/param and a type

  writeln((r, A));   // writes (0.0, [0.0, 0.0, 0.0])
}
{
  proc foo(in x: real, in y: [] real) {
    x = 1.2;  // OK: local copy is modified
    y = 3.4;  // OK: local copy is modified
  }

  var r: real,
      A: [1..3] real;

  foo(r, A);

  writeln((r, A));  // writes (0.0, [0.0, 0.0, 0.0])
}

{
  proc foo(out x: real, out y: [] real) {
    x = 1.2;  // OK: local copy is modified
    y = [3.4, 3.4, 3.4];  // OK: local copy is modified
  }

  var r: real,
      A: [1..3] real;

  foo(r, A);

  writeln((r, A));  // writes (1.2, [3.4, 3.4, 3.4])
}
{
  proc foo(inout x: real, inout y: [] real) {
    x = 1.2;  // OK: local copy is modified
    y = 3.4;  // OK: local copy is modified
  }

  var r: real,
      A: [1..3] real;

  foo(r, A);

  writeln((r, A));  // writes (1.2, [3.4, 3.4, 3.4])
}

