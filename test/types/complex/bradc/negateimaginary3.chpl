
var a = (-0.0, -0.0): complex;

writeln("a is: ", a);

var b = (-2.0, 0.0): complex;

var c = conjg(b);

writeln("c is: ", c);

var d = -2048.0 + 0.0i;

var N = 1024;

var e = conjg(d) / N;

var f = (d.re / N, -d.im / N): complex;

writeln("e is: ", e);

writeln("f is: ", f);

