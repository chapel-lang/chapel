def foo(A: []) where A.rank == 1 && A.eltType == 3*int {
  writeln("1");  
}

def foo(A: []) where A.rank == 2 && A.eltType == 2*int {
  writeln("2");  
}

def foo(A: []) where A.rank == 3 && A.eltType == int {
  writeln("3");  
}

def foo(A: []) where !(A.rank == 1 && A.eltType == 3*int) &&
                     !(A.rank == 2 && A.eltType == 2*int) &&
                     !(A.rank == 3 && A.eltType == int) {
  writeln("4");
}

def bar(A: []) where A.rank == 1 && A.eltType == 3*int {
  writeln("1");  
}

def bar(A: []) where A.rank == 2 && A.eltType == 2*int {
  writeln("2");  
}

def bar(A: []) where A.rank == 3 && A.eltType == int {
  writeln("3");  
}

def bar(A: []) {
  writeln("4");
}

var A: [1..3] 3*int;
foo(A);
bar(A);

var B: [1..3, 1..3] 2*int;
foo(B);
bar(B);

var C: [1..3, 1..3, 1..3] int;
foo(C);
bar(C);

var D: [1..3, 1..3] 3*int;
foo(D);
bar(D);
