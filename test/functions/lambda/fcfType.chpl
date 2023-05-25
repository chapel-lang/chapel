proc f1() { return 1; }
proc f2 { return 2; }
var f3 = 3;
var f4 = lambda() { return 4; };

var a = f1.type:string;
var b = f2.type:string;
var c = f3.type:string;
var d = f4.type:string;

writeln(a);
writeln(b);
writeln(c);
writeln(d);
