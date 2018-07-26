const D = {1..9};

class C {
  var SparseD: sparse subdomain(D);

  proc foo() {
    writeln("SparseD is: ", SparseD);
    SparseD += 4;
    writeln("SparseD is: ", SparseD);
  }
}

var myC = new owned C();
myC.foo();
