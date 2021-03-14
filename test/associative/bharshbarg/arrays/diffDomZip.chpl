config var n = 1;

var A, B : domain(int);

var different = false;

//
// Create two domains with the same indices, where each domain internally
// stores the indices in a different order. The serial iterator will expose
// this difference.
//
// The intention here is to make sure that the parallel iterators do not rely
// on the order of the underlying storage table.
//
while !different {
  A.clear();
  B.clear();

  n += 1;

  //
  // Add in a different order to try and cause hash collisions resulting in
  // a different storage order.
  //
  for i in 1..n do A.add(i);
  for i in 1..n by -1 do B.add(i);

  // If the serial iteration yields in different orders, then we're ready.
  for (aa, bb) in zip(A,B) do
    if aa != bb {
      different = true;
      break;
    }
}

assert(A == B);

//
// Check parallel iterator for domains. Each iteration of the loop body should
// receive the same index from both domains.
//
forall (aa, bb) in zip(A, B) do assert(aa == bb);

var X : [A] int = 1;
var Y : [B] int = 3;

//
// Although the two domains store the indices differently, this zip should work
// because the domains are semantically identical.
//
forall (xx,yy) in zip(X,Y) do xx += yy;

assert(X == 4);
