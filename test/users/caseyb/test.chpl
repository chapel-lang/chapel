use ChapelLocale;

proc main() {
  writeln();
  writeln("In Main().");

  writeln("First we print the results of 'here:'");
  writeln(here);
  writeln();
  writeln("Now we allocate a simple array, var A: [D] real:");
  var D: domain(1) = [1..100];
  var A: [D] real;
  writeln();
  writeln("Now we use the on statement:");
  on (here.getChild()) {
    var A2: [D] real;
  }
  writeln();
  writeln("Now we explicitly set _here:");
  //_here = new subLocale(uid=-2);
  set_here(1);
  writeln(here);
  var A3: [D] real;
  writeln();
}
