use DRimpl;

config const N = 2, M = 3, L=4;

var A2: [1..N, 1..M]  int;
var A3: [1..N, 1..M, 1..L] int;

setup2(A2);
setup3(A3);

proc setup2(A2) {
  var setupVar = 1;
  for (i1,i2) in A2.domain {
    A2[i1,i2] = setupVar;
    setupVar *= 10;
  }
}

proc setup3(A3) {
  var setupVar = 100;
  for (i1,i2,i3) in A3.domain {
    A3[i1,i2,i3] = setupVar + i3;
    if i3 == L then setupVar *= 10;
  }
}

proc d2d(DIMS) {
  var ranges: DIMS.size * range;
  for param dim in 0..DIMS.size-1 do
    ranges(dim) = 
      if isRange(DIMS(dim)) then DIMS(dim)
      else DIMS(dim)..DIMS(dim);
  return {(...ranges)};
}

proc d2u(DIMS, param dim) {
  if dim == DIMS.size-1 then {
    if isRange(DIMS(dim)) then
      return (..,);
    else
      return (DIMS(dim),);
  } else {
    if isRange(DIMS(dim)) then
      return (.., (...d2u(DIMS,dim+1)));
    else
      return (DIMS(dim), (...d2u(DIMS,dim+1)));
  }
}

proc test(ARR,DIMS) {
  writeln();
  writeln("DIMS = ", DIMS);
  var RES1 = plusPR(DIMS, ARR);
  writeln("RES = \n", RES1);
  var RES2 = plusPR(d2d(DIMS), ARR);
  assert(RES2 == RES1);
  var RES3 = plusPR(d2u(DIMS,0), ARR);
  assert(RES3 == RES1);
}

proc main {
  writeln(); writeln(A2);
  test(A2, (1..N, 1..M));
  test(A2, (1, 1..M));
  test(A2, (1..N, 1));
  test(A2, (1, 1));

  writeln(); writeln("---------------------------------");

  writeln(); writeln(A3);
  test(A3, (1..N, 1..M, 1..L));
  test(A3, (1..N, 1..M, 1));
  test(A3, (1, 1..M, 1..L));
  test(A3, (1, 1..M, 1));
  test(A3, (1..N, 1, 1..L));
  test(A3, (1..N, 1, 1));
  test(A3, (1, 1, 1..L));
  test(A3, (1, 1, 1));

  writeln();
}
