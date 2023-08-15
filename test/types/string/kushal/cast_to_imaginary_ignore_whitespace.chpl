use CTypes;
var a = " 9i ";
var a1 = a:imag;
writeln(a1);
var a2 = a:imag(32);
writeln(a2);
var a3 = a:imag(64);
writeln(a3);

var x = " 9i ":c_ptrConst(c_char);
var x1 = x:imag;
writeln(x1);
var x2 = x:imag(32);
writeln(x2);
var x3 = x:imag(64);
writeln(x3);
