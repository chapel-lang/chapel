use LayoutCSR;

config var n = 8;

var d2: domain(2) = {1..n,1..n};
test(2, d2);

// main test driver

proc test(param dim:int, d: domain(dim)) {
  writeln("=== ", dim, " ===");
  // This is our sparse domain that everything will spin around.
  var sd: sparse subdomain(d) dmapped new dmap(new CSR());
  populateDomain(dim, sd);
  writeln("sd(", dim, ") = ", sd);

  var A, B, C: [sd] int;
  var X, Y, Z: [sd] real;

  // conveniences
  proc showA { show(A, "A ="); } proc showB { show(B, "B ="); }
  proc showC { show(C, "C ="); } proc showX { show(X, "X ="); }
  proc showY { show(Y, "Y ="); } proc showZ { show(Z, "Z ="); }
  proc hd(msg:string) { write(msg, "\n "); }

  seed(B, 1); seed(C, 100); seed(Y, 1.0); seed(Z, 100.0);
  writeln("seeded"); showB; showC; showY; showZ;

  forall i in sd { A(i) = sumComps(i); }
  hd("forall(dom) { indexing = }"); showA;

  forall x in X { x = 10; }
  hd("forall(arr) { iterator var = }"); showX;

  forall (i,j) in zip(sd,sd) { A(i) += B(j); }
  hd("forall(dom,dom) { ix = ix }"); showA;

  var D: [sd] int;
  var Q: [sd] real;
  proc showD { show(D, "D ="); } proc showQ { show(Q, "Q ="); }

  forall (a,d) in zip(A,D) { d = a; }
  hd("forall(arr,arr) { ivar = ivar }"); showD;

  forall (i,a) in zip(sd,A) { a = C(i); }
  hd("forall(dom,arr) { ivar = ix }"); showA;

  forall (x,i) in zip(X,sd) { x = Z(i); }
  hd("forall(arr,dom) { ivar = ix }"); showX;

  forall (i,j,k) in zip(sd,sd,sd) { Q(i) = Y(i) + Z(i); }
  hd("forall(dom,dom,dom) { ix = ix, ix }"); showQ;

  forall (i,j,c) in zip(sd,sd,C) { A(i) = B(i) - c; }
  hd("forall(dom,dom,arr) { ix = ix, ivar }"); showA;

  forall (i,z,x) in zip(sd,Z,X) { z = Y(i) - x; }
  hd("forall(dom,arr,arr) { ivar = ix, ivar }"); showZ;

  forall (c,j,k) in zip(C,sd,sd) { A(j) = B(k) + c; }
  hd("forall(arr,dom,dom) { ix = ix, ivar }"); showA;

  forall (z,x,k) in zip(Z,X,sd) { Q(k) = x + z; }
  hd("forall(arr,arr,dom) { ix = ivar, ivar }"); showQ;

  forall (i,j,k,l) in zip(sd,sd,sd,sd) { D(l) = A(i) + B(j) + C(k); }
  hd("forall(dom,dom,dom,dom) { ix = 3*ix }"); showD;

  forall (x,y,z,q) in zip(X,Y,Z,Q) { q = x + y + z; }
  hd("forall(arr,arr,arr,arr) { ivar = 3*ivar }"); showQ;

  forall (i,j,a,b) in zip(sd,sd,A,B) { C(i) = D(j) - a - b; }
  hd("forall(dom,dom,arr,arr) { ix = ix, ivar, ivar }"); showC;

  forall (x,q,i,j) in zip(X,Q,sd,sd) { q = Z(i) - x - Y(j); }
  hd("forall(arr,arr,dom,dom) { ivar = ix, ivar, ix }"); showQ;

  forall (i,a,j,b) in zip(sd,A,sd,B) { D(i) = a - C(i) + b; }
  hd("forall(dom,arr,dom,arr) { ix = ivar, ix, ivar }"); showD;

  forall(x,i,y,j) in zip(X,sd,Y,sd) { Q(i) = Z(i) - x - y; }
  hd("forall(arr,dom,arr,dom) { ix = ix, ivar, ivar }"); showQ;

  A = B; hd("A=B"); showA;
  X = Y; hd("X=Y"); showX;
  D = A; hd("D=A"); showD;
  Q = X; hd("Q=X"); showQ;
  C = D; hd("C=D"); showC;
  B = C; hd("B=C"); showB;
  Z = Q; hd("Z=Q"); showZ;
  Y = Z; hd("Y=Z"); showY;  
}

// helpers

proc populateDomain(param dim, ref sd) where dim == 1 {
  // for 1-D, use half of the values; domain members are not tuples
  for i in 1..n by 2 do sd += i;
}

proc populateDomain(param dim, ref sd) where dim > 1 {
  for i in 1..n-1 {
    var member: index(sd);
    for param dm in 1..dim do member(dm) =
      // feeble attempt at something more sophisticated than just a diagonal
      if dm % 2 == 0 then i else i + 1;
    sd += member;
  }
}

proc seed(A,f) {
  var cnt = f;
  // the following needs to be deterministic so the output is, too
  for a in A { a = cnt; cnt += f; }    
}

proc show(A, msg...) {
  // the following needs to be deterministic so the output is, too
  write((...msg));
  for a in A do write(" ", a);
  writeln();
}

//proc firstComp(arg:int) { return arg; }
//proc firstComp(arg) where isTuple(arg) { return arg(1); }

proc sumComps(arg:int) { return arg; }
proc sumComps(arg) where isTuple(arg) {
  var result: arg(1).type = 0;
  for param ix in 1..arg.size do result += arg(ix);
  return result;
}
