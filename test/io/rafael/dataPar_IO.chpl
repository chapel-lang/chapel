use DistIO;
use BlockDist;
config const n = 30;
config const block = 3;
config const pdebug = false;
//const Space = [1..n,1..n,1..n,1..n,1..n,1..n];
const Space = [1..n];

proc dit(D, A, B, C) {
  forall i in D {
    var (i0,i1,i2,i3,i4,i5) = (i,2,3,4,5,6);
    A(i) = (i0+i1)/i2:int;
    B(i) = (i0+i4)/i5:int;
    C(i) = A(i) + B(i);
    A(i) = 100+i:int;
    B(i) = 200+i:int;
    C(i) = A(i) + B(i);
  }
}

var RA, RB, RC: [Space] int;
for i in Space {
  var (i0,i1,i2,i3,i4,i5) = (i,2,3,4,5,6);
  RA(i) = (i0+i1)/i2:int;
  RB(i) = (i0+i4)/i5:int;
  RC(i) = RA(i) + RB(i);
  RA(i) = 100+i:int;
  RB(i) = 200+i:int;
  RC(i) = RA(i) + RB(i);
}

proc checkdit(distName, A, B, C) {
  var allerr, err = 0;

  for (ra, a) in (RA, A) do
    if ra!=a then err += 1;
  if err>0 {
    writeln(distName, " array A does not match (", err, " errors)");
    allerr += err;
    err = 0;
  }

  for (rb, b) in (RB, B) do
    if rb!=b then err += 1;
  if err>0 {
    writeln(distName, " array B does not match (", err, " errors)");
    allerr += err;
    err = 0;
  }

  for (rc, c) in (RC, C) do
    if rc!=c then err += 1;
  if err>0 then {
    writeln(distName, " array C does not match (", err, " errors)");
    allerr += err;
    err = 0;
  }

  if allerr==0 then writeln(distName, ": OK"); else writeln(distName, ": NO OK");

  return allerr;
}

//const CDom = Space dmapped new dmap(new DistIO(startIdx=(1)));
var Dist = new dmap(new DistIO(startIdx=tuple(1), blocksize=tuple(block)));
var CDom: domain(1) dmapped Dist = Space;

var CA: [CDom] int;
CA._value.setFilename("test5.bor");
//var CA, CB, CC: [CDom] real;
// dit(CDom, CA, CB, CC);
//err += checkdit("DistIO", CA, CB, CC);
  forall i in CDom {
      var (i0,i1,i2,i3,i4,i5) = (i,2,3,4,5,6);
      CA(i) = 500+i:int;
  }

/*
var ll=CA._value.getFilename();
writeln("CA----ll:",ll,"---------------------",CA);
//CA[3..7]=RB[5..9];
CA[3..25]=RB[5..27];
writeln("CA-------------------------",CA);
writeln("Before CA-------------------------");
*/
//                if CDom.isIODist() { writeln("CA is IODist--------------------------------");}
//                if CA.isIODist() { writeln("CA is IODist--------------------------------");}
//                if RB.isBlockDist() { writeln("RB is BlockDist--------------------------------");}
//                if RB.isIODist() { writeln("RB is IODist--------------------------------");}

CA=RB;
if pdebug then writeln("After CA-------------------------",CA);
/*
forall b in RB { b=55; }
if pdebug then writeln("Before RB=CA-------------------------",RB);
RB=CA;
if pdebug then writeln("After RB=CA-------------------------",RB);
*/
//writeln("CC-GetIndices:",CC.dsiGetIndices());

//exit(err);

writeln("Representacion Dist:",Dist);
writeln("Type Dist:",typeToString(Dist.type));
writeln("Type val Dist:",typeToString(Dist._value.type));
writeln("Type CA:",typeToString(CA.type));
writeln("Type val CA:",typeToString(CA._value.type));
writeln("Representacion:",CDom);

