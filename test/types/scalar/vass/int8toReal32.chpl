var p:real(32);
var q:real(32);
var r:real(32);
var s:real(32);

// int literals to real(32)

p = 55;
q = 66;
r = 77;
s = 88;

writeln((p, q, r, s));

var i:int(8) = 11;
var j:uint(8) = 22;
var k:int(16) = 33;
var l:uint(16) = 44;

// int(8)-etc. expressions to real(32)

p = i;
q = j;
r = k;
s = l;

writeln((p, q, r, s));
