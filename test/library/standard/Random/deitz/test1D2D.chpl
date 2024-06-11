use Random;
use BlockDist;
use IO;

config const seed: int = 1;
config const n: int = 8;

{
  var rs = new randomStream(real, seed);

  writeln(for i in 1..n do "%{#.#####}".format(rs.next()));
  writeln(rs.next());
}

{
  var rs = new randomStream(real, seed);

  var A: [1..n] real;
  rs.fill(A);
  writeln(for e in A do "%{#.#####}".format(e));
  writeln(rs.next());
}

{
  var rs = new randomStream(real, seed);

  var A: [1..n/2, 1..2] real;
  rs.fill(A);
  writeln(for e in A do "%{#.#####}".format(e));
  writeln(rs.next());
}

{
  var rs = new randomStream(real, seed);

  var A: [{1..n} dmapped new blockDist(rank=1,boundingBox={1..n})] real;
  rs.fill(A);
  writeln(for e in A do "%{#######}".format(e.locale.id));
  writeln(for e in A do "%{#.#####}".format(e));
  writeln(rs.next());
}

{
  var rs = new randomStream(real, seed);

  var A: [{1..n} dmapped new blockDist(rank=1,boundingBox={1..n/2})] real;
  rs.fill(A);
  writeln(for e in A do "%{#######}".format(e.locale.id));
  writeln(for e in A do "%{#.#####}".format(e));
  writeln(rs.next());
}

{
  var rs = new randomStream(real, seed);

  var A: [{1..n} dmapped new blockDist(rank=1,boundingBox={1..n})] real;
  forall (a,r) in zip(A,rs.next(A.domain)) do
    a = r;
  writeln(for e in A do "%{#######}".format(e.locale.id));
  writeln(for e in A do "%{#.#####}".format(e));
  writeln(rs.next());
}

{
  var rs = new randomStream(real, seed);

  var A: [{1..n} dmapped new blockDist(rank=1,boundingBox={1..n/2})] real;
  forall (a,r) in zip(A,rs.next(A.domain)) do
    a = r;
  writeln(for e in A do "%{#######}".format(e.locale.id));
  writeln(for e in A do "%{#.#####}".format(e));
  writeln(rs.next());
}

{
  var rs = new randomStream(real, seed);

  var A: [{1..n} dmapped new blockDist(rank=1,boundingBox={1..n})] real;
  forall (r,a) in zip(rs.next(A.domain), A) do
    a = r;
  writeln(for e in A do "%{#######}".format(e.locale.id));
  writeln(for e in A do "%{#.#####}".format(e));
  writeln(rs.next());
}

{
  var rs = new randomStream(real, seed);

  var A: [{1..n} dmapped new blockDist(rank=1,boundingBox={1..n/2})] real;
  forall (r,a) in zip(rs.next(A.domain), A) do
    a = r;
  writeln(for e in A do "%{#######}".format(e.locale.id));
  writeln(for e in A do "%{#.#####}".format(e));
  writeln(rs.next());
}
