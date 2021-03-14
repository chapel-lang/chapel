use BlockDist, CyclicDist;
config type myIntType=uint(64);
config const n:myIntType = 3;
const Space = {1:myIntType..n,
               1:myIntType..n,
               1:myIntType..n,
               1:myIntType..n,
               1:myIntType..n,
               1:myIntType..n};

proc dit(D, A, B, C) {
  forall i in D {
    var (i0,i1,i2,i3,i4,i5) = (i);
    A(i) = (i0+i1)/i2:real;
    B(i) = (i3+i4)/i5:real;
    C(i) = A(i) + B(i);
  }
}

var RA, RB, RC: [Space] real;
for i in Space {
  var (i0,i1,i2,i3,i4,i5) = (i);
  RA(i) = (i0+i1)/i2:real;
  RB(i) = (i3+i4)/i5:real;
  RC(i) = RA(i) + RB(i);
}

proc checkdit(distName, A, B, C) {
  var allerr, err = 0;

  for (ra, a) in zip(RA, A) do
    if ra!=a then err += 1;
  if err>0 {
    writeln(distName, " array A does not match (", err, " errors)");
    allerr += err;
    err = 0;
  }

  for (rb, b) in zip(RB, B) do
    if rb!=b then err += 1;
  if err>0 {
    writeln(distName, " array B does not match (", err, " errors)");
    allerr += err;
    err = 0;
  }

  for (rc, c) in zip(RC, C) do
    if rc!=c then err += 1;
  if err>0 then {
    writeln(distName, " array C does not match (", err, " errors)");
    allerr += err;
    err = 0;
  }

  if allerr==0 then writeln(distName, ": OK"); else writeln(distName, ": NO OK");
}

const DDom = Space dmapped defaultDist;
var DA, DB, DC: [DDom] real;
dit(DDom, DA, DB, DC);
checkdit("Default", DA, DB, DC);

const BDom = Space dmapped new dmap(new Block(boundingBox=Space));
var BA, BB, BC: [BDom] real;
dit(BDom, BA, BB, BC);
checkdit("Block", BA, BB, BC);

const CDom = Space dmapped new dmap(new Cyclic(startIdx=(1:myIntType,
                                                         1:myIntType,
                                                         1:myIntType,
                                                         1:myIntType,
                                                         1:myIntType,
                                                         1:myIntType)));
var CA, CB, CC: [CDom] real;
dit(CDom, CA, CB, CC);
checkdit("Cyclic", CA, CB, CC);
