class C {
  def these() {
    for i in 1..10 do
      yield i;
  }

  def these() {
    for i in 1..100 do
      yield i;
  }

}

var myC = new C();

for i in myC do
  writeln("i is: ", i);
