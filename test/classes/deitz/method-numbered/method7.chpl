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

var f : borrowed foo = new borrowed foo();

f.x = 4;
f.print();

printout("I hope this works.");
