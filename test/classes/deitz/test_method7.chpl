function printout(s : string) {
  writeln("The string is '", s, "'");
}

function printout(i : integer) {
  writeln("The integer is ", i);
}

class foo {
  var x : integer;
  function print() {
    printout(x);
  }
}

var f : foo = foo();

f.x = 4;
f.print();

printout("I hope this works.");
