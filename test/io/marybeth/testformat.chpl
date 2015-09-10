var s: string;

s = '%{###.###}';

var i: uint = 1;
var x = 33.44;
var n = 22;
var y = 999.9993;
var z = 899.9999;
var c = (1.0, 1.0):complex;

writeln(s.format(i));
writeln(s.format(x));
writeln(s.format(y));
writeln(s.format(z));
writeln(s.format(n));
writeln("%{####}".format(n));
writeln("%{###########.##}".format(c));

