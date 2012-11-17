use BaseLocale;
use SubLocale;

proc main() {
  writeln();
  writeln("In Main().");

  writeln("First we print the results of 'here:'");
  writeln(here);
  writeln();
  writeln("Now we allocate a simple array, var A: [D] real:");
  var D: domain(1) = {1..100};	// This is allocated on Locale0.
  var A: [D] real;
  writeln();
  var base = new BaseLocale();
  var sub = new SubLocale();
  writeln("Now we use the on statement:");
  on sub {
    writeln(here);
    var A2: [D] real;
  }
  writeln();
  writeln("Now we explicitly set _here:");
  rootLocale.setLocale(0, base);
  writeln(here);
  var A3: [D] real;
  writeln();
//  var handle = here.alloc(8,0,0,"foo");
//  here.free(handle,0,"foo");
}
