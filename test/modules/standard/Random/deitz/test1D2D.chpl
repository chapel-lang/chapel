use Random;
use BlockDist;

config const seed: int = 1;
config const n: int = 8;

{
  var rs = new RandomStream(seed);

  for i in 1..n do writef(" #.#####", rs.getNext());
  writeln();
  writeln(rs.getNext());

  delete rs;
}

{
  var rs = new RandomStream(seed);

  for i in 1..n do writef(" #.#####", rs.getNth(i));
  writeln();
  writeln(rs.getNext());

  delete rs;
}

{
  var rs = new RandomStream(seed);

  var A: [1..n] real;
  rs.fillRandom(A);
  for e in A do writef(" #.#####", e);
  writeln();
  writeln(rs.getNext());

  delete rs;
}

{
  var rs = new RandomStream(seed);

  var A: [1..n/2, 1..2] real;
  rs.fillRandom(A);
  for e in A do writef(" #.#####", e);
  writeln();
  writeln(rs.getNext());

  delete rs;
}

{
  var rs = new RandomStream(seed);

  var A: [{1..n} dmapped Block(rank=1,boundingBox={1..n})] real;
  rs.fillRandom(A);
  for e in A do writef(" #######", e.locale.id);
  writeln();
  for e in A do writef(" #.#####", e);
  writeln();
  writeln(rs.getNext());

  delete rs;
}

{
  var rs = new RandomStream(seed);

  var A: [{1..n} dmapped Block(rank=1,boundingBox={1..n/2})] real;
  rs.fillRandom(A);
  for e in A do writef(" #######", e.locale.id);
  writeln();
  for e in A do writef(" #.#####", e);
  writeln();
  writeln(rs.getNext());

  delete rs;
}

{
  var rs = new RandomStream(seed);

  var A: [{1..n} dmapped Block(rank=1,boundingBox={1..n})] real;
  forall (a,r) in zip(A,rs.iterate(A.domain)) do
    a = r;
  for e in A do writef(" #######", e.locale.id);
  writeln();
  for e in A do writef(" #.#####", e);
  writeln();
  writeln(rs.getNext());

  delete rs;
}

{
  var rs = new RandomStream(seed);

  var A: [{1..n} dmapped Block(rank=1,boundingBox={1..n/2})] real;
  forall (a,r) in zip(A,rs.iterate(A.domain)) do
    a = r;
  for e in A do writef(" #######", e.locale.id);
  writeln();
  for e in A do writef(" #.#####", e);
  writeln();
  writeln(rs.getNext());

  delete rs;
}

{
  var rs = new RandomStream(seed);

  var A: [{1..n} dmapped Block(rank=1,boundingBox={1..n})] real;
  forall (r,a) in zip(rs.iterate(A.domain), A) do
    a = r;
  for e in A do writef(" #######", e.locale.id);
  writeln();
  for e in A do writef(" #.#####", e);
  writeln();
  writeln(rs.getNext());

  delete rs;
}

{
  var rs = new RandomStream(seed);

  var A: [{1..n} dmapped Block(rank=1,boundingBox={1..n/2})] real;
  forall (r,a) in zip(rs.iterate(A.domain), A) do
    a = r;
  for e in A do writef(" #######", e.locale.id);
  writeln();
  for e in A do writef(" #.#####", e);
  writeln();
  writeln(rs.getNext());

  delete rs;
}
