class Matrix {
  type eltType;
}

proc acceptAnyMatrixReal(arg:Matrix(real)) {
  writeln("acceptAnyMatrixReal(", arg.type:string, ")");
}
proc acceptAnyMatrixQ(arg:Matrix(?t)) {
  writeln("acceptAnyMatrixQ(", arg.type:string, ") t=", t:string);
}

proc acceptAnyNilableMatrixReal(arg:Matrix(real)?) {
  writeln("acceptAnyNilableMatrixReal(", arg.type:string, ")");
}
proc acceptAnyNilableMatrixQ(arg:Matrix(?t)?) {
  writeln("acceptAnyNilableMatrixQ(", arg.type:string, ") t=", t:string);
}

proc acceptBorrowedMatrixReal(arg:borrowed Matrix(real)) {
  writeln("acceptBorrowedMatrixReal(", arg.type:string, ")");
}
proc acceptBorrowedMatrixQ(arg:borrowed Matrix(?t)) {
  writeln("acceptBorrowedMatrixQ(", arg.type:string, ") t=", t:string);
}

proc acceptBorrowedNilableMatrixReal(arg:borrowed Matrix(real)?) {
  writeln("acceptBorrowedNilableMatrixReal(", arg.type:string, ")");
}
proc acceptBorrowedNilableMatrixQ(arg:borrowed Matrix(?t)?) {
  writeln("acceptBorrowedNilableMatrixQ(", arg.type:string, ") t=", t:string);
}

proc acceptOwnedMatrixReal(in arg:owned Matrix(real)) {
  writeln("acceptOwnedMatrixReal(", arg.type:string, ")");
}
proc acceptOwnedMatrixQ(in arg:owned Matrix(?t)) {
  writeln("acceptOwnedMatrixQ(", arg.type:string, ") t=", t:string);
}

proc acceptOwnedNilableMatrixReal(in arg:owned Matrix(real)?) {
  writeln("acceptOwnedNilableMatrixReal(", arg.type:string, ")");
}
proc acceptOwnedNilableMatrixQ(in arg:owned Matrix(?t)?) {
  writeln("acceptOwnedNilableMatrixQ(", arg.type:string, ") t=", t:string);
}

proc main() {
  var M = new owned Matrix(real);
  var MN:owned Matrix(real)? = new owned Matrix(real);

  acceptAnyMatrixReal(M);
  acceptAnyMatrixQ(M);

  acceptAnyNilableMatrixReal(MN);
  acceptAnyNilableMatrixQ(MN);

  acceptBorrowedMatrixReal(M);
  acceptBorrowedMatrixQ(M);

  acceptBorrowedNilableMatrixReal(MN);
  acceptBorrowedNilableMatrixQ(MN);

  {
    var MM = new owned Matrix(real);

    acceptOwnedMatrixReal(MM);
  }
  {
    var MM = new owned Matrix(real);

    acceptOwnedMatrixQ(MM);
  }
  {
    var MM = new owned Matrix(real);
    var MMN:owned Matrix(real)? = new owned Matrix(real);

    acceptOwnedNilableMatrixReal(MM);
    acceptOwnedNilableMatrixReal(MMN);
  }
  {
    var MM = new owned Matrix(real);
    var MMN:owned Matrix(real)? = new owned Matrix(real);

    acceptOwnedNilableMatrixQ(MM);
    acceptOwnedNilableMatrixQ(MMN);
  }
}
