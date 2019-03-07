param ITERATIONS = 5;
param SIZE = 100000;

var idomain: domain(uint);

proc icheck( i) {
  writeln( "contains(", i, ") = ", idomain.contains( i));
}

for i in 1..ITERATIONS {
  for d in 1..2*SIZE {       // empty domain
    if (idomain.contains(d:uint)) then
      idomain.remove(d:uint);
  }

  for d in 1..SIZE {         // add everybody
    idomain.add(d:uint);
  }

  for d in 1..SIZE/2 {       // delete half
    if (idomain.contains(d:uint)) then
      idomain.remove(d:uint);
  }

  for d in SIZE+1..2*SIZE {  // add some
    if (idomain.contains(d:uint)) then
      idomain.remove(d:uint);
  }

  for d in 1..SIZE {         // try to add some existing
    if (d%2 == 0) then
      idomain.add(d:uint);
  }

  for d in SIZE/2..SIZE {    // delete some
    if (idomain.contains(d:uint)) then
      idomain.remove(d:uint);
  }
}

icheck(2:uint);
icheck((SIZE/2-2):uint);
icheck((SIZE/2-1):uint);
icheck((SIZE/2):uint);
icheck((SIZE-1):uint);
icheck((SIZE):uint);
icheck((SIZE+1):uint);
icheck((SIZE/4):uint);
