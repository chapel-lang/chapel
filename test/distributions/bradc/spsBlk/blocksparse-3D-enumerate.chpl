use BlockDist;

config const n = 2;

const Elems = {0..#n, 0..#n, 0..#n} dmapped Block({0..#n, 0..#n, 0..#n});

var MatElems: sparse subdomain(Elems);

var elt = (0, 1, 1);
MatElems += elt;

for x in MatElems {
  assert( x == elt );
}

forall x in MatElems {
  assert( x == elt );
}

forall (x,y) in zip(MatElems, MatElems) {
  assert( x == elt );
}

var A:[MatElems] int;

A[elt] = 1;

for x in A {
  assert( x == 1 );
}

forall x in A {
  assert( x == 1 );
}

forall (x,y) in zip(A, A) {
  assert( x == 1 );
  assert( y == 1 );
}

forall (x,y) in zip(MatElems, A) {
  assert( x == elt );
  assert( y == 1 );
}

forall (x,y) in zip(A, MatElems) {
  assert( x == 1 );
  assert( y == elt );
}
