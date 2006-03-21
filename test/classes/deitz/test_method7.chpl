function printout(s : string) {
  writeln("The string is '", s, "'");
}

function printout(i : int) {
  writeln("The int is ", i);
}

class foo {
  var x : int;
  function print() {
    printout(x);
  }
}

var f : foo = foo();

f.x = 4;
f.print();

printout("I hope this works.");
