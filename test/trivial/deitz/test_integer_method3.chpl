fun int(32).print() {
  writeln("int(32).print = ", this);
}

fun int.print() {
  writeln("int.print = ", this);
}

var i : int = 12;
i.print();
var j : int(32) = 14;
j.print();
