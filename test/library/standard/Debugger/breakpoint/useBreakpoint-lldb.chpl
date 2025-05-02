
use Debugger;

// by using a config var, we prevent the compiler from accidentally optimizing
// this out.
config var myVariable: int = 0;

proc main() {

  writeln("myVariable: ", myVariable);

  breakpoint;

  myVariable = 17;

  writeln("myVariable: ", myVariable);

  breakpoint;

  writeln("myVariable: ", myVariable);

}


