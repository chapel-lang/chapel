def printout(s : string) {
  writeln("The string is '", s, "'");
}

def printout(i : int) {
  writeln("The int is ", i);
}

class foo {
  var x : int;
  def print() {
    printout(x);
  }
}

var f : foo = new foo();

f.x = 4;
f.print();

printout("I hope this works.");
