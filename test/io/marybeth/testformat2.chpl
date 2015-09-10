var s: string;

s = '%{###########.###}';

var c = (1.0, 1.0):complex;
var x: imag = 1.0i;
var b = true;

writeln(s.format(c));
writeln(s.format(x));
writeln(s.format(b));
