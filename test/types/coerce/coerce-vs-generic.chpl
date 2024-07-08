proc g(arg)       { writeln("in generic g"); }
proc g(arg: real) { writeln("in real g"); }

g(1);

proc h(arg: integral) { writeln("in generic h"); }
proc h(arg: real)     { writeln("in real h"); }
h(1);
