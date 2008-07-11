var s: sync bool;

def foo() {
  var D: domain(1) = [1..5];
  var A: [D] int;
  begin bar(A);
}

def bar(A: [] int) {
  s;
  writeln(A.domain);
  writeln(A);
}

foo();
s = true;
