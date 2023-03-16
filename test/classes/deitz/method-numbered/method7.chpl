proc printout(s : string) {
  writeln("The string is '", s, "'");
}

proc printout(i : int) {
  writeln("The int is ", i);
}

class foo {
  var x : int;
  proc print() {
    printout(x);
  }
}

var ownF = new owned foo();
var f : borrowed foo = ownF.borrow();

f.x = 4;
f.print();

printout("I hope this works.");
