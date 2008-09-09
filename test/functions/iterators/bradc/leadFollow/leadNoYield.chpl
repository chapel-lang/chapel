class C {
  def these() {
    for i in 1..10 do
      yield i;
  }

  def these(leader) {
  }
}

var myC = new C();

for i in myC do
  writeln("i is: ", i);







