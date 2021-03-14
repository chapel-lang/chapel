var D = {1..10};
var A:[D] int;

proc f(in dom) {
  D = {1..3};
  writeln("D is ", D);
  writeln("dom is ", dom);
}

writeln("passing D");
D = {1..10};
f(D);

writeln("passing A.domain");
D = {1..10};
f(A.domain);
