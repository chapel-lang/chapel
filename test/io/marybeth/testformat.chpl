var s: string;

s = '%{###.###}';

var i: uint = 1;
var x = 33.44;
var n = 22;
var y = 999.9993;
var z = 899.9999;
var c = (1.0, 1.0):complex;

writeln(s.format(s,i));
writeln(s.format(s,x));
writeln(s.format(s,y));
writeln(s.format(s,z));
writeln(s.format(s,n));
writeln("%{####}".format(n));
writeln("%{###########.##}".format(c));

