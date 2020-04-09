
use StencilDist;

config const n = 10;

proc main() {
  var D = {1..n, 1..n};
  var Space = D dmapped Stencil(D, fluff=(1,1,));
  var A : [Space] int;

  var c = 1;
  for a in A {
    a = c;
    c += 1;
  }

  //
  // Writing before a call to updateFluff used to result in confusing and
  // surprising output:
  //
  // ```
  // 1 2 3 4 5 0 7 8 9 10
  // 11 12 13 14 15 0 17 18 19 20
  // 21 22 23 24 25 0 27 28 29 30
  // 31 32 33 34 35 0 37 38 39 40
  // 41 42 43 44 45 0 47 48 49 50
  // 0 0 0 0 0 0 57 58 59 60
  // 61 62 63 64 65 66 67 68 69 70
  // 71 72 73 74 75 76 77 78 79 80
  // 81 82 83 84 85 86 87 88 89 90
  // 91 92 93 94 95 96 97 98 99 100
  // ```
  //
  // Note the '0' values. These are the cached values currently on Locale0.
  // This occurred because StencilArr.dsiSerialWrite only *read* values, which
  // would pull from the cache when possible.
  //
  // The correct solution is to always read from the actual element.
  //

  writeln(A);
}
