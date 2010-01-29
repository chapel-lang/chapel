use driver;

const TD1D: domain(1) = Space1 - (1);
def foo1D(A: [TD1D] int) {
  var errs = 0;
  for i in [TD1D] do
    if A[i].locale != A1D[i+1].locale {
      writeln("A[",i,"] Incorrect reindex");
      errs += 1;
    }
  return errs;
}
for e in A2D do e = next();
writeln("A1D: ", foo1D(A1D), " errors");

const TD2D: domain(2) = Space2 - (1, 1);
def foo2D(A: [TD2D] int) {
  var errs = 0;
  for (i,j) in [TD2D] do
    if A[i,j].locale != A2D[i+1,j+1].locale {
      writeln("A[",i,",",j,"] Incorrect reindex");
      errs += 1;
    }
  return errs;
}
for e in A2D do e = next();
writeln("A2D: ", foo2D(A2D), " errors");

const TD3D: domain(3) = Space3 - (1, 1, 1);
def foo3D(A: [TD3D] int) {
  var errs = 0;
  for (i,j,k) in [TD3D] do
    if A[i,j,k].locale != A3D[i+1,j+1,k+1].locale {
      writeln("A[",i,",",j,",",k,"] Incorrect reindex");
      errs += 1;
    }
  return errs;
}
for e in A3D do e = next();
writeln("A3D: ", foo3D(A3D), " errors");

const TD4D: domain(4) = Space4 - (1, 1, 1, 1);
def foo4D(A: [TD4D] int) {
  var errs = 0;
  for (i,j,k,l) in [TD4D] do
    if A[i,j,k,l].locale != A4D[i+1,j+1,k+1,l+1].locale {
      writeln("A[",i,",",j,",",k,",",l,"] Incorrect reindex");
      errs += 1;
    }
  return errs;
}
for e in A4D do e = next();
writeln("A4D: ", foo4D(A4D), " errors");

const TD2D64: domain(2,int(64)) = Space2D64 - (1:int(64), 1:int(64));
def foo2D64(A: [TD2D64] int) {
  var errs = 0;
  for (i,j) in [TD2D64] do
    if A[i,j].locale != A2D64[i+1,j+1].locale {
      writeln("A[",i,",",j,"] Incorrect reindex");
      errs += 1;
    }
  return errs;
}
for e in A2D64 do e = next();
writeln("A2D64: ", foo2D64(A2D64), " errors");
