proc f() { writeln("side effect"); return 2; }

var x, y, z = f();

writeln((x, y, z));

var i = 0;
proc next() { i = i + 1; return i; }

var a, b, c = next();

writeln((a, b, c));
