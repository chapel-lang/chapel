const D = {1..9};

class C {
  var SparseD: sparse subdomain(D);
  var A: [SparseD] real;

  proc foo() {
    writeln("SparseD is: ", SparseD);
    writeln("A is: ", A);
    SparseD += 4;
    writeln("SparseD is: ", SparseD);
    writeln("A is: ", A);
    A(4) = 1.2;
    writeln("SparseD is: ", SparseD);
    writeln("A is: ", A);
  }
}

var myC = new unmanaged C();
myC.foo();
delete myC;
