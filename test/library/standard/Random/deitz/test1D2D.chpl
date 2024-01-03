use Random;
use BlockDist;
use IO;

config const seed: int = 1;
config const n: int = 8;

{
  var rs = new randomStream(real, seed);

  writeln(for i in 1..n do "%{#.#####}".format(rs.getNext()));
  writeln(rs.getNext());
}

{
  var rs = new randomStream(real, seed);

  writeln(for i in 0..<n do "%{#.#####}".format(rs.getNth(i)));
  writeln(rs.getNext());
}

{
  var rs = new randomStream(real, seed);

  var A: [1..n] real;
  rs.fill(A);
  writeln(for e in A do "%{#.#####}".format(e));
  writeln(rs.getNext());
}

{
  var rs = new randomStream(real, seed);

  var A: [1..n/2, 1..2] real;
  rs.fill(A);
  writeln(for e in A do "%{#.#####}".format(e));
  writeln(rs.getNext());
}

{
  var rs = new randomStream(real, seed);

  var A: [{1..n} dmapped blockDist(rank=1,boundingBox={1..n})] real;
  rs.fill(A);
  writeln(for e in A do "%{#######}".format(e.locale.id));
  writeln(for e in A do "%{#.#####}".format(e));
  writeln(rs.getNext());
}

{
  var rs = new randomStream(real, seed);

  var A: [{1..n} dmapped blockDist(rank=1,boundingBox={1..n/2})] real;
  rs.fill(A);
  writeln(for e in A do "%{#######}".format(e.locale.id));
  writeln(for e in A do "%{#.#####}".format(e));
  writeln(rs.getNext());
}

{
  var rs = new randomStream(real, seed);

  var A: [{1..n} dmapped blockDist(rank=1,boundingBox={1..n})] real;
  forall (a,r) in zip(A,rs.iterate(A.domain)) do
    a = r;
  writeln(for e in A do "%{#######}".format(e.locale.id));
  writeln(for e in A do "%{#.#####}".format(e));
  writeln(rs.getNext());
}

{
  var rs = new randomStream(real, seed);

  var A: [{1..n} dmapped blockDist(rank=1,boundingBox={1..n/2})] real;
  forall (a,r) in zip(A,rs.iterate(A.domain)) do
    a = r;
  writeln(for e in A do "%{#######}".format(e.locale.id));
  writeln(for e in A do "%{#.#####}".format(e));
  writeln(rs.getNext());
}

{
  var rs = new randomStream(real, seed);

  var A: [{1..n} dmapped blockDist(rank=1,boundingBox={1..n})] real;
  forall (r,a) in zip(rs.iterate(A.domain), A) do
    a = r;
  writeln(for e in A do "%{#######}".format(e.locale.id));
  writeln(for e in A do "%{#.#####}".format(e));
  writeln(rs.getNext());
}

{
  var rs = new randomStream(real, seed);

  var A: [{1..n} dmapped blockDist(rank=1,boundingBox={1..n/2})] real;
  forall (r,a) in zip(rs.iterate(A.domain), A) do
    a = r;
  writeln(for e in A do "%{#######}".format(e.locale.id));
  writeln(for e in A do "%{#.#####}".format(e));
  writeln(rs.getNext());
}
