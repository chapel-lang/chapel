fun printout(s : string) {
  writeln("The string is '", s, "'");
}

fun printout(i : int) {
  writeln("The int is ", i);
}

class foo {
  var x : int;
  fun print() {
    printout(x);
  }
}

var f : foo = foo();

f.x = 4;
f.print();

printout("I hope this works.");
