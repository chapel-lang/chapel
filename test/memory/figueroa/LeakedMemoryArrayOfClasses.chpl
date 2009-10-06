use Memory;

class C {
  var x: int;
}

def foo() {
  var c: [i in 1..10] C = new C();
  for cc in c do delete cc;
}

serial true {
  var m1 = memoryUsed();
  foo();
  var m2 = memoryUsed();
  writeln("Amount of leaked memory after deleting array c: ", m2-m1);
}
