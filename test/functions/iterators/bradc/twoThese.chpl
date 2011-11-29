class C {
  iter these() {
    for i in 1..10 do
      yield i;
  }

  iter these() {
    for i in 1..100 do
      yield i;
  }

}

var myC = new C();

for i in myC do
  writeln("i is: ", i);
