var s: sync bool;

proc foo() {
  var D: domain(1) = {1..5};
  var A: [D] int;
  begin bar(A);
}

proc bar(A: [] int) {
  s;
  writeln(A.domain);
  writeln(A);
}

foo();
s = true;
