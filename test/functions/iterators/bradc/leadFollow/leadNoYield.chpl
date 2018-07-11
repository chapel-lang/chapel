class C {
  iter these() {
    for i in 1..10 do
      yield i;
  }

  proc these(leader) {
  }
}

var myC = new unmanaged C();

for i in myC do
  writeln("i is: ", i);


delete myC;




