config param parSafe = true;
config const debug = false;
config type myType = int;
config const n:myType = 7:myType;

proc checkit(d1, d2) {
  if debug {
    writeln(d1);
    writeln(d2);
  }
  writeln(d1==d2);
  writeln(d1!=d2);
}

var D1: domain(myType, parSafe=parSafe);
var D2: domain(myType, parSafe=parSafe);

checkit(D1, D2);

D1 += (n+n):myType;
D1 += (n+n+n):myType;
D1 += (n+n+n+n):myType;
D1 += (n+n+n+n+n):myType;
D1 += (n+n+n+n+n+n):myType;
D1 += (n+n+n+n+n+n+n):myType;
D1 += (n+n+n+n+n+n+n+n):myType;
D1 += (n+n+n+n+n+n+n+n+n):myType;
D1 += (n+n+n+n+n+n+n+n+n+n):myType;

checkit(D1, D2);

D2 += (n+n+n):myType;
D2 += (n+n+n+n+n):myType;
D2 += (n+n+n+n+n+n+n):myType;
D2 += (n+n+n+n+n+n+n+n):myType;
D2 += (n+n+n+n+n+n):myType;
D2 += (n+n+n+n+n+n+n+n+n+n):myType;
D2 += (n+n+n+n):myType;
D2 += (n+n+n+n+n+n+n+n+n):myType;
D2 += (n+n):myType;

checkit(D1, D2);

